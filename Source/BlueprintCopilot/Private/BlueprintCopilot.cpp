// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "BlueprintCopilot.h"
#include "BlueprintCopilotWidget.h"

#include "EditorUtilityWidget.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"

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
	EditorWidget->SetNumberOfTestButtonPressed(NumberOfTestButtonPressed);
}


