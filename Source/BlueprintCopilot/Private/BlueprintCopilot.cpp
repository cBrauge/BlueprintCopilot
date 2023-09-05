// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilot.h"
#include "BlueprintCopilotWidget.h"

#include "EditorUtilityWidget.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"

#include "Editor.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "K2Node_VariableGet.h"
#include "K2Node_VariableSet.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "Engine/SimpleConstructionScript.h"
#include "Kismet/KismetMathLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include <optional>
#include <string>
#include <unordered_map>

// TODO move those into helper cache class
// Make functions take an additional parameter for the internal naming of the node in the cache,
// not reusing the same nodes as it can clash in the future if we create 2 nodes for same property
using BlueprintID = std::string;
using NodeID = std::string;
std::unordered_map<BlueprintID, UBlueprint *> blueprint_cache{};
std::unordered_map<NodeID, UK2Node *> node_cache{};

// TODO also get an already created blueprint
auto CreateBlueprint(const std::string &name, const BlueprintID& blueprintID)
{
    auto targetPackage{CreatePackage(*FString::Printf(TEXT("/Game/BlueprintCopilotGeneratedBlueprints/%s"), *FString(name.c_str())))};

    auto blueprint{FKismetEditorUtilities::CreateBlueprint(
        AActor::StaticClass(),
        targetPackage,
        *FString(name.c_str()),
        BPTYPE_Normal,
        UBlueprint::StaticClass(),
        UBlueprintGeneratedClass::StaticClass())};

    blueprint_cache[blueprintID] = blueprint;

    return blueprint;
}

bool AddVariable(const BlueprintID& blueprintID, const std::string &name, const FName type, const std::string &value = "")
{
    auto blueprint{blueprint_cache[blueprintID]};
    FEdGraphPinType PinType;
    PinType.PinCategory = type;

    if (!value.empty())
    {
        return FBlueprintEditorUtils::AddMemberVariable(blueprint, name.c_str(), PinType, value.c_str());
    }
    else
    {
        return FBlueprintEditorUtils::AddMemberVariable(blueprint, name.c_str(), PinType);
    }
}

UEdGraph *GetGraph(const BlueprintID& blueprintID)
{
    auto blueprint{blueprint_cache[blueprintID]};
    if (blueprint->UbergraphPages.Num() == 0)
    {
        return nullptr;
    }

    return blueprint->UbergraphPages[0];
}

FProperty *GetProperty(const BlueprintID& blueprintID, const std::string &name)
{
    auto blueprint{blueprint_cache[blueprintID]};

    const auto fstring{FString(name.c_str())};
    // TODO faster way?
    for (TFieldIterator<FProperty> PropIt(blueprint->GeneratedClass); PropIt; ++PropIt)
    {
        if (PropIt->GetName() == fstring)
        {
            return *PropIt;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("Failed to find Blueprint property!"));
    return nullptr;
}

UK2Node_VariableGet *AddVariableGetNode(const BlueprintID& blueprintID, const std::string &propertyName, const NodeID &nodeID)
{
    auto blueprint{blueprint_cache[blueprintID]};

    auto eventGraph{GetGraph(blueprintID)};
    auto property{GetProperty(blueprintID, propertyName)};

    if (!property)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find one or more Blueprint properties!"));
        return nullptr;
    }

    auto node = NewObject<UK2Node_VariableGet>(eventGraph);
    node->VariableReference.SetFromField<FProperty>(property, false);
    node->AllocateDefaultPins();
    eventGraph->AddNode(node);

    node_cache[nodeID] = node;

    return node;
}

UK2Node_CallFunction *AddFunctionNode(const BlueprintID& blueprintID, const std::string &functionName, const NodeID &nodeID)
{
    auto blueprint{blueprint_cache[blueprintID]};

    auto eventGraph{GetGraph(blueprintID)};
    FString functionFString = FString(functionName.c_str());
    // TODO check outside of math class and have to specify to openai
    auto function{UKismetMathLibrary::StaticClass()->FindFunctionByName(*functionFString)};
    if (!function)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find function %s"), *functionFString);
        return nullptr;
    }

    auto node{NewObject<UK2Node_CallFunction>(eventGraph)};
    node->SetFromFunction(function);
    node->AllocateDefaultPins();
    eventGraph->AddNode(node);

    node_cache[nodeID] = node;

    return node;
}

UK2Node_VariableSet *AddVariableSetNode(const BlueprintID& blueprintID, const std::string &propertyName, const NodeID &nodeID)
{
    auto blueprint{blueprint_cache[blueprintID]};
    auto eventGraph{GetGraph(blueprintID)};
    auto property{GetProperty(blueprintID, propertyName)};

    if (!property)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find one or more Blueprint properties!"));
        return nullptr;
    }

    auto node{NewObject<UK2Node_VariableSet>(eventGraph)};
    node->VariableReference.SetFromField<FProperty>(property, false);
    node->AllocateDefaultPins();
    eventGraph->AddNode(node);

    node_cache[nodeID] = node;

    return node;
}

// Use this if you're not sure of the pins of a node
void LogPins(UK2Node *node)
{
    for (auto pin : node->Pins)
    {
        UE_LOG(LogTemp, Warning, TEXT("Available Pin: %s"), *pin->PinName.ToString());
    }
}

auto ConnectPins(const NodeID &outputNodeID, const std::string &outputPinName, const NodeID &inputNodeID, const std::string &inputPinName)
{
    auto outputNode{node_cache[outputNodeID]};
    auto inputNode{node_cache[inputNodeID]};

    auto outputPinfName{FString(outputPinName.c_str())};
    auto outputPin = outputNode->FindPin(outputPinfName);
    auto inputPinfName{FString(inputPinName.c_str())};
    auto inputPin = inputNode->FindPin(inputPinfName);

    if (!outputPin)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find output pin: %s"), *outputPinfName);
        return false;
    }

    if (!inputPin)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find input pin: %s"), *inputPinfName);
        return false;
    }

    outputPin->MakeLinkTo(inputPin);
    return true;
}

bool PositionNode(const NodeID &nodeID, int x, int y)
{
    auto node{node_cache[nodeID]};

    node->NodePosX = x;
    node->NodePosY = y;
    return true;
}

void UpdateBlueprint(const BlueprintID& blueprintID)
{
    auto blueprint{blueprint_cache[blueprintID]};

    FCompilerResultsLog Results;
    FBlueprintEditorUtils::MarkBlueprintAsModified(blueprint);
    blueprint->SimpleConstructionScript->Modify();
    blueprint->PostEditChange();
    FKismetEditorUtilities::CompileBlueprint(blueprint, EBlueprintCompileOptions::None, &Results);
}

void CreateBlueprintPermanently(const BlueprintID& blueprintID)
{
    UpdateBlueprint(blueprintID);

    auto blueprint{blueprint_cache[blueprintID]};

    FAssetRegistryModule::AssetCreated(blueprint);
    blueprint->MarkPackageDirty();
}

void Foo()
{
    // 1. Create a new Blueprint
    const auto blueprintName{ "TestBLUEPRINT" };
    const auto blueprintID{"TestBLUEPRINT0"};
    const auto myVarName{"MyVariable"};
    const auto myVarNodeID{"MyVariableNode0"};
    const auto myOtherVarName{"MyOtherVariable"};
    const auto myOtherVarNodeID{"MyOtherVariableNode0"};
    const auto myResultName{"MyResult"};
    const auto myResultNodeID{"MyResultNode0"};
    const auto functionName{"Add_IntInt"};
    const auto functionNodeID{"Add_IntIntNode0"};

    auto NewBlueprint{CreateBlueprint(blueprintName, blueprintID)};
    if (!NewBlueprint)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create blueprint!"));
        return;
    }

    // 2. Create Variables
    auto MyVarSuccess{AddVariable(blueprintID, myVarName, UEdGraphSchema_K2::PC_Int, "5")};
    if (!MyVarSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add MyVar to blueprint!"));
        return;
    }

    auto MyOtherVarSuccess{AddVariable(blueprintID, myOtherVarName, UEdGraphSchema_K2::PC_Int, "10")};
    if (!MyOtherVarSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add MyOtherVar to blueprint!"));
        return;
    }

    auto MyResSuccess{AddVariable(blueprintID, myResultName, UEdGraphSchema_K2::PC_Int)};
    if (!MyResSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add MyRes to blueprint!"));
        return;
    }

    UpdateBlueprint(blueprintID);

    auto MyVarNode{AddVariableGetNode(blueprintID, myVarName, myVarNodeID)};
    auto MyOtherVarNode{AddVariableGetNode(blueprintID, myOtherVarName, myOtherVarNodeID)};

    auto AdditionNode{AddFunctionNode(blueprintID, functionName, functionNodeID)};
    if (!AdditionNode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add AddFunctionNode"));
        return;
    }

    auto SetMyResultNode{AddVariableSetNode(blueprintID, myResultName, myResultNodeID)};
    if (!SetMyResultNode)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add SetMyResultNode"));
        return;
    }

    UpdateBlueprint(blueprintID);

    // 4. Connect all the nodes together

    // Connect the MyVariable getter to the addition node
    auto MyVarToAddSuccess{ConnectPins(myVarNodeID, myVarName, functionNodeID, "A")};
    if (!MyVarToAddSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect MyVariable to addition node"));
        return;
    }

    auto MyOtherVarToAddSuccess{ConnectPins(myOtherVarNodeID, myOtherVarName, functionNodeID, "B")};
    if (!MyOtherVarToAddSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect MyOtherVariable to addition node"));
        return;
    }

    // Connect the addition node to the MyResult setter
    auto AdditionToSetResultSuccess{ConnectPins(functionNodeID, "ReturnValue", myResultNodeID, myResultName)};
    if (!AdditionToSetResultSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect addition result to MyResult setter"));
        return;
    }

    // Optionally, if you want to organize the nodes visually, adjust their positions
    PositionNode(myVarNodeID, 0, 0);
    PositionNode(myOtherVarNodeID, 0, 200);
    PositionNode(functionNodeID, 200, 100);
    PositionNode(myResultNodeID, 400, 100);

    // Compile the blueprint after modifying

    CreateBlueprintPermanently(blueprintID);

    UE_LOG(LogTemp, Log, TEXT("Blueprint created and modified!"));
}

void UBlueprintCopilot::Init()
{
    // Put initialization code here
}

void UBlueprintCopilot::InitializeTheWidget()
{
    // Initialize the widget here
    EditorWidget->SetNumberOfTestButtonPressed(NumberOfTestButtonPressed);

    // Bind all required delegates to the Widget.
    EditorWidget->OnTestButtonPressedDelegate.BindUObject(this, &UBlueprintCopilot::OnTestButtonPressed);
}

void UBlueprintCopilot::OnTestButtonPressed()
{
    // Button on the widget has been pressed. Increase the counter and inform the widget about it.
    NumberOfTestButtonPressed++;
    Foo();
    EditorWidget->SetNumberOfTestButtonPressed(NumberOfTestButtonPressed);
}
