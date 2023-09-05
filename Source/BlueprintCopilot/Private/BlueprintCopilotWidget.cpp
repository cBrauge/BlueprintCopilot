// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilotWidget.h"

void UBlueprintCopilotWidget::TestButtonPressed()
{
	OnTestButtonPressedDelegate.ExecuteIfBound();
}
