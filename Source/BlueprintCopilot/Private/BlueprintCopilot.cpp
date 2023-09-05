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

#include "LibBlueprintCopilot.h"
#include "ChatGPT.h"
#include "LibLLMFactory.h"
#include "ActionsDeserialization.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

// TODO move those into helper cache class
// Make functions take an additional parameter for the internal naming of the node in the cache,
// not reusing the same nodes as it can clash in the future if we create 2 nodes for same property
using BlueprintID = std::string;
using NodeID = std::string;
std::unordered_map<BlueprintID, UBlueprint *> blueprint_cache{};
std::unordered_map<NodeID, UK2Node *> node_cache{};

// TODO also get an already created blueprint
auto CreateBlueprint(const std::string &name, const BlueprintID &blueprintID)
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

bool AddVariable(const BlueprintID &blueprintID, const std::string &name, const FName type, const std::string &value = "")
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

UEdGraph *GetGraph(const BlueprintID &blueprintID)
{
    auto blueprint{blueprint_cache[blueprintID]};
    if (blueprint->UbergraphPages.Num() == 0)
    {
        return nullptr;
    }

    return blueprint->UbergraphPages[0];
}

FProperty *GetProperty(const BlueprintID &blueprintID, const std::string &name)
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

UK2Node_VariableGet *AddVariableGetNode(const BlueprintID &blueprintID, const std::string &propertyName, const NodeID &nodeID)
{
    auto blueprint{blueprint_cache[blueprintID]};

    auto eventGraph{GetGraph(blueprintID)};
    auto property{GetProperty(blueprintID, propertyName)};

    if (!property)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find Blueprint properties!"));
        return nullptr;
    }

    auto node = NewObject<UK2Node_VariableGet>(eventGraph);
    node->VariableReference.SetFromField<FProperty>(property, false);
    node->AllocateDefaultPins();
    eventGraph->AddNode(node);

    node_cache[nodeID] = node;

    return node;
}

UK2Node_CallFunction *AddFunctionNode(const BlueprintID &blueprintID, const std::string &functionName, const NodeID &nodeID)
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

UK2Node_VariableSet *AddVariableSetNode(const BlueprintID &blueprintID, const std::string &propertyName, const NodeID &nodeID)
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

bool UpdateBlueprint(const BlueprintID &blueprintID)
{
    auto blueprint{blueprint_cache[blueprintID]};

    FCompilerResultsLog Results;
    FBlueprintEditorUtils::MarkBlueprintAsModified(blueprint);
    blueprint->SimpleConstructionScript->Modify();
    blueprint->PostEditChange();
    FKismetEditorUtilities::CompileBlueprint(blueprint, EBlueprintCompileOptions::None, &Results);

    return true;
}

bool CreateBlueprintPermanently(const BlueprintID &blueprintID)
{
    UpdateBlueprint(blueprintID);

    auto blueprint{blueprint_cache[blueprintID]};

    FAssetRegistryModule::AssetCreated(blueprint);
    blueprint->MarkPackageDirty();

    return true;
}

void Foo()
{
    // 1. Create a new Blueprint
    const auto blueprintName{"TestBLUEPRINT"};
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

bool PerformAction(const LibBlueprintCopilot::Guidance::CreateBlueprint &action)
{
    auto NewBlueprint{CreateBlueprint(action.BlueprintName, action.BlueprintID)};
    if (!NewBlueprint)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create blueprint!"));
        return false;
    }

    return true;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::CreateLink &action)
{
    const auto success{ConnectPins(action.SourceNodeID, action.SourcePinName, action.DestinationNodeID, action.DestinationPinName)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create link"));
        return success;
    }

    return success;
}

auto TypeToSchemaType(const std::string &name)
{
    if (name == "UEdGraphSchema_K2::PC_Int")
    {
        return UEdGraphSchema_K2::PC_Int;
    }

    // TODO support more types
    return UEdGraphSchema_K2::PC_Int;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::AddVariable &action)
{
    const auto type{TypeToSchemaType(action.PinCategory)};
    const auto success{AddVariable(action.BlueprintID, action.VariableName, type, action.DefaultValue)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add variable"));
        return success;
    }

    return success;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::AddVariableGetNode &action)
{
    const auto success{AddVariableGetNode(action.BlueprintID, action.PropertyName, action.NodeID)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add variable Get node"));
        return false;
    }

    return true;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::AddVariableSetNode &action)
{
    const auto success{AddVariableSetNode(action.BlueprintID, action.PropertyName, action.NodeID)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add variable Set node"));
        return false;
    }

    return true;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::AddFunctionNode &action)
{
    const auto success{AddFunctionNode(action.BlueprintID, action.FunctionName, action.NodeID)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to add function node"));
        return false;
    }

    return true;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::PositionNode &action)
{
    const auto success{PositionNode(action.NodeID, action.x, action.y)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to position node"));
        return success;
    }

    return success;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::UpdateBlueprint &action)
{
    const auto success{UpdateBlueprint(action.BlueprintID)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to update blueprint"));
        return success;
    }

    return success;
}

bool PerformAction(const LibBlueprintCopilot::Guidance::CreateBlueprintPermanently &action)
{
    const auto success{CreateBlueprintPermanently(action.BlueprintID)};
    if (!success)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create blueprint on disk"));
        return success;
    }

    return success;
}

void Foo2()
{
    auto libLLM{LibBlueprintCopilot::Guidance::LibLLMFactory::CreateLibLLM(LibBlueprintCopilot::Guidance::LibLLMModel::FakeLLMModel, "")};

    std::string jsonString = R"(
[
  {
    "Type": "CreateBlueprint",
    "BlueprintName": "TestBLUEPRINT",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Creating the blueprint named 'TestBLUEPRINT'."
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyVariable",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "5",
    "Reasoning": "Adding 'MyVariable' of type int with default value 5."
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyOtherVariable",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "10",
    "Reasoning": "Adding 'MyOtherVariable' of type int with default value 10."
  },
  {
    "Type": "AddVariable",
    "BlueprintID": "ID_TestBLUEPRINT",
    "VariableName": "MyResult",
    "PinCategory": "UEdGraphSchema_K2::PC_Int",
    "DefaultValue": "",
    "Reasoning": "Adding 'MyResult' variable to store the result of the addition."
  },
 {
    "Type": "UpdateBlueprint",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Compiling and updating the blueprint after changes."
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyVariable",
    "NodeID": "ID_GetNode_MyVariable",
    "Reasoning": "Creating a getter node for 'MyVariable'."
  },
  {
    "Type": "AddVariableGetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyOtherVariable",
    "NodeID": "ID_GetNode_MyOtherVariable",
    "Reasoning": "Creating a getter node for 'MyOtherVariable'."
  },
  {
    "Type": "AddFunctionNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "FunctionName": "Add_IntInt",
    "NodeID": "ID_AddFunctionNode",
    "Reasoning": "Creating a node for addition operation."
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_GetNode_MyVariable",
    "SourcePinName": "MyVariable",
    "DestinationNodeID": "ID_AddFunctionNode",
    "DestinationPinName": "A",
    "Reasoning": "Linking 'MyVariable' to the addition function input A."
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_GetNode_MyOtherVariable",
    "SourcePinName": "MyOtherVariable",
    "DestinationNodeID": "ID_AddFunctionNode",
    "DestinationPinName": "B",
    "Reasoning": "Linking 'MyOtherVariable' to the addition function input B."
  },
  {
    "Type": "AddVariableSetNode",
    "BlueprintID": "ID_TestBLUEPRINT",
    "PropertyName": "MyResult",
    "NodeID": "ID_SetNode_MyResult",
    "Reasoning": "Creating a setter node for 'MyResult'."
  },
  {
    "Type": "CreateLink",
    "SourceNodeID": "ID_AddFunctionNode",
    "SourcePinName": "ReturnValue",
    "DestinationNodeID": "ID_SetNode_MyResult",
    "DestinationPinName": "MyResult",
    "Reasoning": "Linking the result of the addition to the 'MyResult' setter."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_GetNode_MyVariable",
    "x": 100,
    "y": 100,
    "Reasoning": "Positioning the getter node for 'MyVariable'."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_GetNode_MyOtherVariable",
    "x": 100,
    "y": 200,
    "Reasoning": "Positioning the getter node for 'MyOtherVariable'."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_AddFunctionNode",
    "x": 250,
    "y": 150,
    "Reasoning": "Positioning the addition function node."
  },
  {
    "Type": "PositionNode",
    "NodeID": "ID_SetNode_MyResult",
    "x": 400,
    "y": 150,
    "Reasoning": "Positioning the setter node for 'MyResult'."
  },
  {
    "Type": "UpdateBlueprint",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Compiling and updating the blueprint after changes."
  },
  {
    "Type": "CreateBlueprintPermanently",
    "BlueprintID": "ID_TestBLUEPRINT",
    "Reasoning": "Saving the blueprint on disk to make it permanent."
  }
]

    )";

    auto response{libLLM->Request(jsonString)};
    auto actions{LibBlueprintCopilot::Guidance::ConvertResponseToActions(response)};
    int i = 0;
    for (const auto &action : actions)
    {
        UE_LOG(LogTemp, Warning, TEXT("Starting action: %d"), i++);

        auto success{std::visit([](const auto &a) -> bool
                                { return PerformAction(a); },
                                action)};
        if (!success)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed!"));

            return;
        }
    }
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
    Foo2();
    EditorWidget->SetNumberOfTestButtonPressed(NumberOfTestButtonPressed);
}
