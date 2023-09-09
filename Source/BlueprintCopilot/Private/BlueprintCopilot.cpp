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

using namespace LibBlueprintCopilot::Guidance;

// TODO move those inside the settings and widget
LibLLMModel Model{LibLLMModel::FakeLLMModel};
std::string APIKey{""};

/// @brief Calls LLM and execute the instructions
/// @param model Model to use
/// @param api_key API Key to the llm service
/// @param user_input What the user wants to do, if using the fake model, simply put the json array of actions
/// @param gpt_model Kind of GPT model to use: 'gpt-4', 'gpt-3.5-turbo'...
void Execute(LibLLMModel model, std::string_view api_key, std::string user_input, std::string_view gpt_model)
{
    const auto libLLM{LibLLMFactory::CreateLibLLM(model, api_key)};

    const auto response{libLLM->Request(user_input, gpt_model)};
    const auto actions{ConvertResponseToActions(response)};
    int i{0};
    for (const auto& action : actions)
    {
        // TODO check manual action

        const auto success{std::visit(
            [&i](const auto& a) -> bool {
                UE_LOG(
                    LogTemp, Warning, TEXT("BlueprintCopilot: Action #%d: %s"), i++, *FString(PrettyPrint(a).c_str()));
                return PerformAction(a);
            },
            action)};
        if (!success)
        {
            UE_LOG(LogTemp, Error, TEXT("BlueprintCopilot: Failed!"));
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

void UBlueprintCopilot::OnTestButtonPressed(FString APIModel, FString GPTModel, FString UserInput)
{
    UE_LOG(LogTemp, Error, TEXT("BlueprintCopilot: Selected API Model: %s"), *APIModel);
    UE_LOG(LogTemp, Error, TEXT("BlueprintCopilot: Selected GPT Model: %s"), *GPTModel);
    UE_LOG(LogTemp, Error, TEXT("BlueprintCopilot: User input: %s"), *UserInput);

    const auto model{APIModel == "Fake" ? LibLLMModel::FakeLLMModel : LibLLMModel::OpenAILLMModel};
    const auto convertedGPTModel{std::string(TCHAR_TO_UTF8(*GPTModel))};
    // const auto convertedAPIKey{std::string(TCHAR_TO_UTF8(*APIKey))};
    const auto convertedUserInput{std::string(TCHAR_TO_UTF8(*UserInput))};
    NumberOfTestButtonPressed++;
    Execute(model, APIKey, convertedUserInput, convertedGPTModel);
    EditorWidget->SetNumberOfTestButtonPressed(NumberOfTestButtonPressed);
}
