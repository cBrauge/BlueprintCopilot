#ifndef _LibBlueprintCopilot_module_h_
#define _LibBlueprintCopilot_module_h_

#include "Engine.h"
// #include "UnrealEd.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FLibBlueprintCopilotEditorModule : public IModuleInterface
{
public:
    void StartupModule() override;

    void ShutdownModule() override;
};

#endif //_LibBlueprintCopilot_module_h_