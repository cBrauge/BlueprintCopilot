// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "BlueprintCopilotWidget.h"

void UBlueprintCopilotWidget::TestButtonPressed()
{
	OnTestButtonPressedDelegate.ExecuteIfBound();
}
