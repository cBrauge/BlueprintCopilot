// Copyright (c) 2022 Ren Brauge. All rights reserved.

#pragma once

#include "BlueprintCopilotBase.h"
#include "BlueprintCopilot.generated.h"

/**
 * Editor object which handles all of the logic of the Plugin.
 */

UCLASS()
class BLUEPRINTCOPILOT_API UBlueprintCopilot : public UBlueprintCopilotBase
{

    GENERATED_BODY()

public:
    // UBlueprintCopilotBase implementation
    void Init() override;

protected:
    // UBlueprintCopilotBase implementation
    void InitializeTheWidget();

public:
    /**
	 * Called when the test button has been pressed on the widget.
	 */
    void OnTestButtonPressed(FString APIModel, FString GPTModel, FString UserInput);

    FString TimeMessage;
    FString ErrorMessage;
};
