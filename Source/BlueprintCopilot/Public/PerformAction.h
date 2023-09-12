// Copyright (c) 2022 Ren Brauge. All rights reserved.
#pragma once

#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/UserWidget.h"
#include "BlueprintCopilotWidget.h"
#include "Editor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Engine/SimpleConstructionScript.h"
#include "K2Node_CallFunction.h"
#include "K2Node_VariableGet.h"
#include "K2Node_VariableSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "LevelEditor.h"
#include "ObjectCache.h"
#include "WidgetBlueprint.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Internationalization/Text.h"

using namespace LibBlueprintCopilot;

static ObjectCache objectCache{};

void HandleBlueprintNotFound(const BlueprintID& blueprintID)
{
    auto message{FString::Printf(TEXT("Failed to find BlueprintID: %s"), *FString(blueprintID.c_str()))};
    UE_LOG(LogTemp, Error, TEXT("%s"), *message);
}

void HandleNodeNotFound(const NodeID& nodeID)
{
    auto message{FString::Printf(TEXT("Failed to find NodeID: %s"), *FString(nodeID.c_str()))};
    UE_LOG(LogTemp, Error, TEXT("%s"), *message);
}

UBlueprint* CreateBlueprint(const std::string& name, const BlueprintID& blueprintID, const BlueprintType& type)
{
    UClass* baseClass               = nullptr;
    UClass* blueprintClass          = nullptr;
    UClass* blueprintGeneratedClass = nullptr;

    if (type == "Actor")
    {
        baseClass               = AActor::StaticClass();
        blueprintClass          = UBlueprint::StaticClass();
        blueprintGeneratedClass = UBlueprintGeneratedClass::StaticClass();
    }
    else if (type == "Widget")
    {
        baseClass               = UUserWidget::StaticClass();
        blueprintClass          = UWidgetBlueprint::StaticClass();
        blueprintGeneratedClass = UBlueprintGeneratedClass::StaticClass();
    }
    else
    {
        auto message{FString::Printf(TEXT("Type of blueprint not supported: %s"), *FString(type.c_str()))};
        UE_LOG(LogTemp, Error, TEXT("%s"), *message);
        return nullptr;
    }

    auto targetPackage{
        CreatePackage(*FString::Printf(TEXT("/Game/BlueprintCopilotGeneratedBlueprints/%s"), *FString(name.c_str())))};

    auto blueprint{FKismetEditorUtilities::CreateBlueprint(
        baseClass, targetPackage, *FString(name.c_str()), BPTYPE_Normal, blueprintClass, blueprintGeneratedClass)};

    objectCache.UpsertBlueprint(blueprintID, blueprint);

    return blueprint;
}

bool UpdateBlueprint(const BlueprintID& blueprintID)
{
    UE_LOG(LogTemp, Error, TEXT("UpdateBlueprint Called for: %s"), *FString(blueprintID.c_str()));
    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint.has_value())
    {
        HandleBlueprintNotFound(blueprintID);
        UE_LOG(LogTemp, Error, TEXT("UpdateBlueprint Called for: %s but not found"), *FString(blueprintID.c_str()));
        return false;
    }

    FCompilerResultsLog Results;
    FBlueprintEditorUtils::MarkBlueprintAsModified(blueprint.value());
    if (blueprint.value()->SimpleConstructionScript)
    {
        blueprint.value()->SimpleConstructionScript->Modify();
    }

    blueprint.value()->PostEditChange();
    FKismetEditorUtilities::CompileBlueprint(blueprint.value(), EBlueprintCompileOptions::None, &Results);

    return true;
}

bool AddVariable(
    const BlueprintID& blueprintID, const std::string& name, const FName type, const std::string& value = "")
{
    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint)
    {
        HandleBlueprintNotFound(blueprintID);
        return false;
    }

    FEdGraphPinType PinType;
    PinType.PinCategory = type;
    bool success        = false;

    if (!value.empty())
    {
        success = FBlueprintEditorUtils::AddMemberVariable(*blueprint, name.c_str(), PinType, value.c_str());
    }
    else
    {
        success = FBlueprintEditorUtils::AddMemberVariable(*blueprint, name.c_str(), PinType);
    }

    UpdateBlueprint(blueprintID);
    return success;
}

UEdGraph* GetGraph(const BlueprintID& blueprintID)
{
    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint.has_value())
    {
        HandleBlueprintNotFound(blueprintID);
        return nullptr;
    }

    if (blueprint.value()->UbergraphPages.Num() == 0)
    {
        return nullptr;
    }

    return blueprint.value()->UbergraphPages[0];
}

FProperty* GetProperty(const BlueprintID& blueprintID, const std::string& name)
{
    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint.has_value())
    {
        HandleBlueprintNotFound(blueprintID);
        return nullptr;
    }

    const auto fstring{FString(name.c_str())};
    // TODO faster way?
    for (TFieldIterator<FProperty> PropIt(blueprint.value()->GeneratedClass); PropIt; ++PropIt)
    {
        if (PropIt->GetName() == fstring)
        {
            return *PropIt;
        }
    }

    UE_LOG(LogTemp, Error, TEXT("GetProperty: Failed to find Blueprint property: %s"), *FString(blueprintID.c_str()));
    return nullptr;
}

UK2Node_VariableGet* AddVariableGetNode(
    const BlueprintID& blueprintID, const std::string& propertyName, const NodeID& nodeID)
{
    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint.has_value())
    {
        UE_LOG(
            LogTemp, Error, TEXT("AddVariableGetNode did not found blueprint id: %s"), *FString(blueprintID.c_str()));

        HandleBlueprintNotFound(blueprintID);
        return nullptr;
    }
    ::UpdateBlueprint(blueprintID);

    auto eventGraph{GetGraph(blueprintID)};
    auto property{GetProperty(blueprintID, propertyName)};

    if (!property)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find Blueprint property: %s"), *FString(propertyName.c_str()));
        return nullptr;
    }

    auto node{NewObject<UK2Node_VariableGet>(eventGraph)};
    node->VariableReference.SetFromField<FProperty>(property, false);
    node->AllocateDefaultPins();
    eventGraph->AddNode(node);

    objectCache.UpsertNode(nodeID, node);

    return node;
}

UK2Node_CallFunction* AddFunctionNode(
    const BlueprintID& blueprintID, const std::string& functionName, const NodeID& nodeID)
{
    auto eventGraph{GetGraph(blueprintID)};
    auto functionFString{FString(functionName.c_str())};
    // TODO check outside of math class and have to specify to openai
    const auto kismetMathLibraryNamespace{std::string{"UKismetMathLibrary::"}};
    if (functionName._Starts_with(kismetMathLibraryNamespace))
    {
        auto realFunctionName{FString(functionName.substr(kismetMathLibraryNamespace.size()).c_str())};
        auto function{UKismetMathLibrary::StaticClass()->FindFunctionByName(*realFunctionName)};
        if (!function)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to find function %s"), *functionFString);
            return nullptr;
        }

        auto node{NewObject<UK2Node_CallFunction>(eventGraph)};
        node->SetFromFunction(function);
        node->AllocateDefaultPins();
        eventGraph->AddNode(node);

        objectCache.UpsertNode(nodeID, node);

        return node;
    }

    UE_LOG(LogTemp, Error, TEXT("Function instantiation not supported %s"), *functionFString);
    return nullptr;
}

UK2Node_VariableSet* AddVariableSetNode(
    const BlueprintID& blueprintID, const std::string& propertyName, const NodeID& nodeID)
{
    auto eventGraph{GetGraph(blueprintID)};
    auto property{GetProperty(blueprintID, propertyName)};

    if (!property)
    {
        UE_LOG(LogTemp, Error, TEXT("AddVariableSetNode: Failed to find one or more Blueprint property: %s"),
            *FString(propertyName.c_str()));
        return nullptr;
    }

    auto node{NewObject<UK2Node_VariableSet>(eventGraph)};
    node->VariableReference.SetFromField<FProperty>(property, false);
    node->AllocateDefaultPins();
    eventGraph->AddNode(node);

    objectCache.UpsertNode(nodeID, node);

    return node;
}

// Use this if you're not sure of the pins of a node
void LogPins(UK2Node* node)
{
    for (auto pin : node->Pins)
    {
        UE_LOG(LogTemp, Warning, TEXT("Available Pin: %s"), *pin->PinName.ToString());
    }
}

auto ConnectPins(const NodeID& outputNodeID, const std::string& outputPinName, const NodeID& inputNodeID,
    const std::string& inputPinName)
{
    auto outputNode{objectCache.GetNode(outputNodeID)};
    auto inputNode{objectCache.GetNode(inputNodeID)};

    if (!outputNode.has_value())
    {
        HandleNodeNotFound(outputNodeID);
        return false;
    }

    if (!inputNode.has_value())
    {
        HandleNodeNotFound(inputNodeID);
        return false;
    }

    auto outputPinfName{FString(outputPinName.c_str())};
    auto outputPin = outputNode.value()->FindPin(outputPinfName);
    auto inputPinfName{FString(inputPinName.c_str())};
    auto inputPin = inputNode.value()->FindPin(inputPinfName);

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

bool AddTextBlockToWidgetBlueprint(const BlueprintID& blueprintID, FString text)
{
    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint.has_value())
    {
        HandleBlueprintNotFound(blueprintID);
        return false;
    }

    // Ensure it's a WidgetBlueprint
    auto widgetBlueprint = Cast<UWidgetBlueprint>(blueprint.value());
    if (!widgetBlueprint)
    {
        // The provided blueprint is not a WidgetBlueprint
        auto message{FString::Printf(TEXT("Blueprint %s is not a widget blueprint, can't add UI to it"), *FString(blueprintID.c_str()))};
        UE_LOG(LogTemp, Error, TEXT("%s"), *message);
        return false;
    }

    auto widgetTree = widgetBlueprint->WidgetTree;
    if (!widgetTree)
    {
        auto message{FString::Printf(TEXT("Blueprint %s is a widget blueprint, but doesn't have a valid WidgetTree"), *FString(blueprintID.c_str()))};
        UE_LOG(LogTemp, Error, TEXT("%s"), *message);
        return false;
    }

    // Create a new TextBlock
    auto newTextBlock = widgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), NAME_None);
    if (!newTextBlock)
    {
        // Failed to construct the TextBlock
        auto message{FString::Printf(TEXT("Blueprint %s failed to create text block"), *FString(blueprintID.c_str()))};
        UE_LOG(LogTemp, Error, TEXT("%s"), *message);
        return false;
    }

    newTextBlock->SetText(FText::FromString(text));

    widgetTree->RootWidget = newTextBlock;

    UpdateBlueprint(blueprintID);

    return true;
}

bool PositionNode(const NodeID& nodeID, int x, int y)
{
    auto node{objectCache.GetNode(nodeID)};
    if (!node.has_value())
    {
        HandleNodeNotFound(nodeID);
        return false;
    }

    node.value()->NodePosX = x;
    node.value()->NodePosY = y;
    return true;
}

bool CreateBlueprintPermanently(const BlueprintID& blueprintID)
{
    ::UpdateBlueprint(blueprintID);

    auto blueprint{objectCache.GetBlueprint(blueprintID)};
    if (!blueprint.has_value())
    {
        HandleBlueprintNotFound(blueprintID);
        return false;
    }

    FAssetRegistryModule::AssetCreated(blueprint.value());
    blueprint.value()->MarkPackageDirty();

    return true;
}

void LogAndDisplayError(UBlueprintCopilotWidget* widget, const FString& message)
{
    UE_LOG(LogTemp, Error, TEXT("%s"), *message);
    widget->SetErrorMessage(message);
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::CreateBlueprint& action)
{
    auto NewBlueprint{::CreateBlueprint(action.BlueprintName, action.BlueprintID, action.BlueprintType)};
    if (!NewBlueprint)
    {
        auto message{FString::Printf(TEXT("Failed to create blueprint, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return false;
    }

    return true;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const Guidance::CreateLink& action)
{
    const auto success{
        ConnectPins(action.SourceNodeID, action.SourcePinName, action.DestinationNodeID, action.DestinationPinName)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to create link, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return success;
    }

    return success;
}

auto TypeToSchemaType(const std::string& name)
{
    if (name == "UEdGraphSchema_K2::PC_Int")
    {
        return UEdGraphSchema_K2::PC_Int;
    }

    // TODO support more types
    return UEdGraphSchema_K2::PC_Int;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::AddVariable& action)
{
    const auto type{TypeToSchemaType(action.PinCategory)};
    const auto success{::AddVariable(action.BlueprintID, action.VariableName, type, action.DefaultValue)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to add variable, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return success;
    }

    return success;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::AddVariableGetNode& action)
{
    const auto success{::AddVariableGetNode(action.BlueprintID, action.PropertyName, action.NodeID)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to add variable Get node, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return false;
    }

    return true;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::AddVariableSetNode& action)
{
    const auto success{::AddVariableSetNode(action.BlueprintID, action.PropertyName, action.NodeID)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to add variable Set node, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return false;
    }

    return true;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::AddFunctionNode& action)
{
    const auto success{::AddFunctionNode(action.BlueprintID, action.FunctionName, action.NodeID)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to add function node, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return false;
    }

    return true;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::PositionNode& action)
{
    const auto success{::PositionNode(action.NodeID, action.x, action.y)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to position node, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return success;
    }

    return success;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::UpdateBlueprint& action)
{
    const auto success{::UpdateBlueprint(action.BlueprintID)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to update blueprint, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return success;
    }

    return success;
}

bool PerformAction(UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::ManualOperation& action)
{
    auto message{FString::Printf(TEXT("Can't perform action over manual operation"))};
    LogAndDisplayError(widget, message);
    return false;
}

bool PerformAction(
    UBlueprintCopilotWidget* widget, const LibBlueprintCopilot::Guidance::CreateBlueprintPermanently& action)
{
    const auto success{::CreateBlueprintPermanently(action.BlueprintID)};
    if (!success)
    {
        auto message{FString::Printf(TEXT("Failed to create blueprint on disk, check logs for more details"))};
        LogAndDisplayError(widget, message);
        return success;
    }

    return success;
}
