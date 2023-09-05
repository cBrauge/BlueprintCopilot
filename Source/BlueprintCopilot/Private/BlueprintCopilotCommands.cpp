// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilotCommands.h"

#include "EditorStyleSet.h"

FBlueprintCopilotCommands::FBlueprintCopilotCommands()
    : TCommands<FBlueprintCopilotCommands>(
        TEXT("BlueprintCopilot Commands"), FText::FromString(TEXT("Commands to control BlueprintCopilot")), NAME_None,
#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1))
        FAppStyle::GetAppStyleSetName()
#else
        FEditorStyle::GetStyleSetName()
#endif
    )
{
}

void FBlueprintCopilotCommands::RegisterCommands()
{
#define LOCTEXT_NAMESPACE "BlueprintCopilotLoc"
    UI_COMMAND(OpenBlueprintCopilotWindow, "BlueprintCopilot", "Opens BlueprintCopilot Window",
        EUserInterfaceActionType::Check, FInputChord(EModifierKey::Shift | EModifierKey::Alt, EKeys::M));
#undef LOCTEXT_NAMESPACE
}