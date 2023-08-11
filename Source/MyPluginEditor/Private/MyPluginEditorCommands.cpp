// Copyright (c) 2022 Damian Nowakowski. All rights reserved.

#include "MyPluginEditorCommands.h"
#include "EditorStyleSet.h"

FMyPluginEditorCommands::FMyPluginEditorCommands() :
	TCommands<FMyPluginEditorCommands>(
		TEXT("My Plugin Commands"), 
		FText::FromString(TEXT("Commands to control My Plugin")), 
		NAME_None, 
#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1))
		FAppStyle::GetAppStyleSetName()
#else
		FEditorStyle::GetStyleSetName()
#endif
	)
{}

void FMyPluginEditorCommands::RegisterCommands()
{
#define LOCTEXT_NAMESPACE "MyPluginLoc"
	UI_COMMAND(OpenMyPluginWindow, "My Plugin", "Opens My Plugin Window", EUserInterfaceActionType::Check, FInputChord(EModifierKey::Shift | EModifierKey::Alt, EKeys::M));
#undef LOCTEXT_NAMESPACE
}