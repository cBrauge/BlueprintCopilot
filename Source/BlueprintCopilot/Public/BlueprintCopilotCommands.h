// Copyright (c) 2022 Ren Brauge. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

/**
 * Class handling UICommands of the editor.
 * Currently only "Open BlueprintCopilot" commands is required.
 * It is
 * done via commands, because we want to have a keyboard shortcut for it.
 */

class BLUEPRINTCOPILOT_API FBlueprintCopilotCommands : public TCommands<FBlueprintCopilotCommands>
{

public:
    FBlueprintCopilotCommands();
    void RegisterCommands() override;
    TSharedPtr<FUICommandInfo> OpenBlueprintCopilotWindow;
};
