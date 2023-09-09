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
// DECLARE_DELEGATE(FOnTestButton);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTestButton, FString, APIModel, FString, GPTModel, FString, UserInput);
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
    void SetNumberOfTestButtonPressed(int32 NewNumber);

    /**
     * "Global Namespace" option has been changed on the Widget.
     */
    UFUNCTION(BlueprintCallable, category = "Blueprint")
    void TestButtonPressed(FString APIModel, FString GPTModel, FString UserInput);

    // UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    // class UComboBoxString* GPTModelChoice;

    // UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    // class UComboBoxString* APIModelChoice;

    // UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    // class UMultiLineEditableText* UserInput;

    /**
     * Callbacks to be binded to the BlueprintCopilot methods.
     */
    FOnTestButton OnTestButtonPressedDelegate;
};