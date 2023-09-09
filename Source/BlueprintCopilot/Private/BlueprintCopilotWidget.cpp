// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilotWidget.h"

#include "Components/ComboBoxString.h"

void UBlueprintCopilotWidget::TestButtonPressed(FString APIModel, FString GPTModel, FString UserInput)
{
    OnTestButtonPressedDelegate.ExecuteIfBound(APIModel, GPTModel, UserInput);
}
