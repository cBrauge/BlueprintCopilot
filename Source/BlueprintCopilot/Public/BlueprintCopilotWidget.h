// Copyright (c) 2022 Ren Brauge. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "BlueprintCopilotWidget.generated.h"

/**
 * Widget code used to communicate between code and BP widget.
 */

/**
 * Declaration of all required delegates for a proper communication with UI.
 * They will be broadcasted to inform
 * the plugin code about changes in UI.
 */
DECLARE_DELEGATE_ThreeParams(FOnTestButton, FString, FString, FString);

UCLASS()
class BLUEPRINTCOPILOT_API UBlueprintCopilotWidget : public UEditorUtilityWidget
{
    GENERATED_BODY()

public:
    /**
     * Set "Global Namespace" option to the Widget.
     */
    UFUNCTION(BlueprintImplementableEvent)
    void SetErrorMessage(const FString& message);

    UFUNCTION(BlueprintImplementableEvent)
    void SetTime(const FString& time);

    /**
     * "Global Namespace" option has been changed on the Widget.
     */
    UFUNCTION(BlueprintCallable, category = "Blueprint")
    void TestButtonPressed(FString APIModel, FString GPTModel, FString UserInput);

    /**
     * Callbacks to be binded to the BlueprintCopilot methods.
     */
    FOnTestButton OnTestButtonPressedDelegate;
};