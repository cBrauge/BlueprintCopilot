// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilot.h"

#include "ActionsDeserialization.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "BlueprintCopilotWidget.h"
#include "Editor.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Engine/SimpleConstructionScript.h"
#include "Examples.h"
#include "K2Node_CallFunction.h"
#include "K2Node_VariableGet.h"
#include "K2Node_VariableSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "KismetCompiler.h"
#include "LevelEditor.h"
#include "LibLLMFactory.h"
#include "PerformAction.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

using namespace LibBlueprintCopilot;

bool DoesNotContainManualOperations(UBlueprintCopilotWidget* widget, const std::vector<Guidance::Action>& actions)
{
    const auto containsManualOperation{std::any_of(actions.begin(), actions.end(),
        [](const Guidance::Action& action) { return std::holds_alternative<Guidance::ManualOperation>(action); })};

    if (!containsManualOperation)
    {
        return true;
    }

    std::ostringstream oss;

    oss << "Can't execute commands, it contains manual operations, you should report what you were trying to do "
           "and what manual operations was created to the creator of the plugin:"
        << std::endl;
    for (const auto& action : actions)
    {
        if (!std::holds_alternative<Guidance::ManualOperation>(action))
        {
            continue;
        }

        oss << PrettyPrint(std::get<Guidance::ManualOperation>(action)) << std::endl << std::endl;
    }

    widget->SetErrorMessage(FString(oss.str().c_str()));
    return false;
}

/// @brief Calls LLM and execute the instructions
/// @param model Model to use
/// @param api_key API Key to the llm service
/// @param user_input What the user wants to do, if using the fake model, simply put the json array of actions
/// @param gpt_model Kind of GPT model to use: 'gpt-4', 'gpt-3.5-turbo'...
bool Execute(UBlueprintCopilotWidget* widget, Guidance::LibLLMModel model, std::string_view api_key,
    std::string user_input, std::string_view gpt_model, std::string_view azure_resource_name, std::string_view azure_deployment_id, std::string_view azure_api_version)
{
    if (model == Guidance::LibLLMModel::OpenAILLMModel && api_key == "")
    {
        auto message{FString::Printf(TEXT("BlueprintCopilot: You first need to put your OpenAI APIKey in settings"))};
        UE_LOG(LogTemp, Error, TEXT("%s"), *message);
        widget->SetErrorMessage(message);
        return false;
    }
    else if (model == Guidance::LibLLMModel::AzureLLMModel && (api_key == "" || azure_resource_name == "" || azure_deployment_id == "" || azure_api_version == ""))
    {
        auto message{FString::Printf(TEXT("BlueprintCopilot: You first need to put your Azure APIKey, Azure Resource Name, Azure Deployment Id and Azure API Version in settings"))};
        UE_LOG(LogTemp, Error, TEXT("%s"), *message);
        widget->SetErrorMessage(message);
        return false;
    }

    const auto libLLM{Guidance::LibLLMFactory::CreateLibLLM(model, api_key, azure_resource_name, azure_deployment_id, azure_api_version)};

    const auto response{libLLM->Request(user_input, gpt_model)};
    UE_LOG(LogTemp, Log, TEXT("BlueprintCopilot: Raw response: %s"), *FString(response.c_str()));

    const auto actions{Guidance::ConvertResponseToActions(response)};
    if (std::holds_alternative<Guidance::ParseError>(actions))
    {
        auto parseError{FString(std::get<Guidance::ParseError>(actions).Reason.c_str())};
        UE_LOG(LogTemp, Error, TEXT("BlueprintCopilot: Failed to parse response of OpenAI: %s"), *parseError);
        widget->SetErrorMessage(parseError);
        return false;
    }

    // Ensure there are no ManualOperation
    if (!DoesNotContainManualOperations(widget, std::get<std::vector<Guidance::Action>>(actions)))
    {
        return false;
    }

    int i{0};
    for (const auto& action : std::get<std::vector<Guidance::Action>>(actions))
    {
        const auto success{std::visit(
            [&](const auto& a) -> bool {
                UE_LOG(LogTemp, Log, TEXT("BlueprintCopilot: Action #%d: %s"), i, *FString(PrettyPrint(a).c_str()));
                return PerformAction(widget, a);
            },
            action)};
        if (!success)
        {
            UE_LOG(LogTemp, Error, TEXT("BlueprintCopilot: Failed at action #%d"), i);
            return false;
        }

        i++;
    }

    return true;
}

void UBlueprintCopilot::Init()
{
    // Put initialization code here
}

void UBlueprintCopilot::InitializeTheWidget()
{
    // Initialize the widget here
    EditorWidget->SetErrorMessage("");

    // Bind all required delegates to the Widget.
    EditorWidget->OnTestButtonPressedDelegate.BindUObject(this, &UBlueprintCopilot::OnTestButtonPressed);
}

void UBlueprintCopilot::OnTestButtonPressed(FString APIModel, FString GPTModel, FString UserInput)
{
    auto startTime = std::chrono::high_resolution_clock::now();
    try
    {
        EditorWidget->SetTime("Execution started... It can take a while");
        auto pluginSettings{GetMutableDefault<UBlueprintCopilotSettings>()};

        UE_LOG(LogTemp, Log, TEXT("BlueprintCopilot: Selected API Model: %s"), *APIModel);
        UE_LOG(LogTemp, Log, TEXT("BlueprintCopilot: Selected GPT Model: %s"), *GPTModel);
        UE_LOG(LogTemp, Log, TEXT("BlueprintCopilot: User input: %s"), *UserInput);

        const auto model{
            APIModel == "Fake" ? Guidance::LibLLMModel::FakeLLMModel : APIModel == "Azure" ? Guidance::LibLLMModel::AzureLLMModel : Guidance::LibLLMModel::OpenAILLMModel};
        const auto convertedGPTModel{std::string(TCHAR_TO_UTF8(*GPTModel))};
        const auto convertedAPIKey{std::string(TCHAR_TO_UTF8(*(pluginSettings->APIKey)))};
        const auto convertedUserInput{std::string(TCHAR_TO_UTF8(*UserInput))};
        const auto convertedAzureResourceName{std::string(TCHAR_TO_UTF8(*(pluginSettings->AzureResourceName)))};
        const auto convertedAzureDeploymentId{std::string(TCHAR_TO_UTF8(*(pluginSettings->AzureDeploymentId)))};
        const auto convertedAzureAPIVersion{std::string(TCHAR_TO_UTF8(*(pluginSettings->AzureAPIVersion)))};

        auto success{Execute(EditorWidget, model, convertedAPIKey, convertedUserInput, convertedGPTModel, convertedAzureResourceName, convertedAzureDeploymentId, convertedAzureAPIVersion)};
        // UBlueprint* blueprint{CreateBlueprint("Test", "0", "Widget")};
        // AddTextBlockToWidgetBlueprint("0", "This is a test");
        // CreateBlueprintPermanently("0");
        // auto success{true};

        auto finishTime = std::chrono::high_resolution_clock::now();
        auto duration   = std::chrono::duration_cast<std::chrono::seconds>(finishTime - startTime);
        int minutes     = duration.count() / 60;
        int seconds     = duration.count() % 60;
        auto time       = FString::Printf(TEXT("%d:%d min"), minutes, seconds);
        EditorWidget->SetTime(time);
        UE_LOG(LogTemp, Log, TEXT("%s"), *time);

        if (success)
        {
            EditorWidget->SetErrorMessage("Finished successfuly");
        }
    }
    catch (std::exception& e)
    {
        auto finishTime = std::chrono::high_resolution_clock::now();
        auto duration   = std::chrono::duration_cast<std::chrono::seconds>(finishTime - startTime);
        int minutes     = duration.count() / 60;
        int seconds     = duration.count() % 60;
        auto time       = FString::Printf(TEXT("%d:%d min"), minutes, seconds);
        UE_LOG(LogTemp, Error, TEXT("%s"), *time);

        auto error = FString::Printf(TEXT("Caught exception: %s"), *FString(e.what()));
        UE_LOG(LogTemp, Error, TEXT("%s"), *error);
        EditorWidget->SetErrorMessage(error);
    }
}
