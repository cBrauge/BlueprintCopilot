// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilotModule.h"
#include "BlueprintCopilot.h"
#include "BlueprintCopilotCommands.h"
#include "BlueprintCopilotStyle.h"

#include "Widgets/Docking/SDockTab.h"
#include "Framework/Docking/TabManager.h"
#include "Interfaces/IMainFrameModule.h"

#include "PropertyEditorModule.h"
#include "LevelEditor.h"

IMPLEMENT_MODULE(FBlueprintCopilotModule, FBlueprintCopilot)
DEFINE_LOG_CATEGORY(BlueprintCopilot2)

// Id of the BlueprintCopilot Tab used to spawn and observe this tab.
const FName BlueprintCopilotTabId = FName(TEXT("BlueprintCopilot"));

void FBlueprintCopilotModule::StartupModule()
{
	// Register Styles.
	FBlueprintCopilotStyle::Initialize();
	FBlueprintCopilotStyle::ReloadTextures();

	// Register UICommands.
	FBlueprintCopilotCommands::Register();

	// Register OnPostEngineInit delegate.
	OnPostEngineInitDelegateHandle = FCoreDelegates::OnPostEngineInit.AddRaw(this, &FBlueprintCopilotModule::OnPostEngineInit);

	// Create and initialize Editor object.
	Editor = NewObject<UBlueprintCopilotBase>(GetTransientPackage(), UBlueprintCopilot::StaticClass());
	Editor->Init();

	// Register Tab Spawner. Do not show it in menu, as it will be invoked manually by a UICommand.
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		BlueprintCopilotTabId,
		FOnSpawnTab::CreateRaw(this, &FBlueprintCopilotModule::SpawnEditor),
		FCanSpawnTab::CreateLambda([this](const FSpawnTabArgs& Args) -> bool { return CanSpawnEditor(); })
	)
	.SetMenuType(ETabSpawnerMenuType::Hidden)
	.SetIcon(FSlateIcon(FBlueprintCopilotStyle::GetStyleSetName(), "BlueprintCopilot.MenuIcon"));
}

void FBlueprintCopilotModule::ShutdownModule()
{
	// Unregister Tab Spawner
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(BlueprintCopilotTabId);

	// Cleanup the Editor object.
	Editor = nullptr;

	// Remove OnPostEngineInit delegate
	FCoreDelegates::OnPostEngineInit.Remove(OnPostEngineInitDelegateHandle);

	// Unregister UICommands.
	FBlueprintCopilotCommands::Unregister();

	// Unregister Styles.
	FBlueprintCopilotStyle::Shutdown();
}

void FBlueprintCopilotModule::OnPostEngineInit()
{
	// This function is for registering UICommand to the engine, so it can be executed via keyboard shortcut.
	// This will also add this UICommand to the menu, so it can also be executed from there.
	
	// This function is valid only if no Commandlet or game is running. It also requires Slate Application to be initialized.
	if ((IsRunningCommandlet() == false) && (IsRunningGame() == false) && FSlateApplication::IsInitialized())
	{
		if (FLevelEditorModule* LevelEditor = FModuleManager::LoadModulePtr<FLevelEditorModule>(TEXT("LevelEditor")))
		{
			// Create a UICommandList and map editor spawning function to the UICommand of opening BlueprintCopilot Editor.
			TSharedPtr<FUICommandList> Commands = MakeShareable(new FUICommandList());
			Commands->MapAction(
				FBlueprintCopilotCommands::Get().OpenBlueprintCopilotWindow,
				FExecuteAction::CreateRaw(this, &FBlueprintCopilotModule::InvokeEditorSpawn),
				FCanExecuteAction::CreateRaw(this, &FBlueprintCopilotModule::CanSpawnEditor),
				FIsActionChecked::CreateRaw(this, &FBlueprintCopilotModule::IsEditorSpawned)
			);

			// Register this UICommandList to the MainFrame.
			// Otherwise nothing will handle the input to trigger this command.
			IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
			MainFrame.GetMainFrameCommandBindings()->Append(Commands.ToSharedRef());

			// Create a Menu Extender, which adds a button that executes the UICommandList of opening BlueprintCopilot Window.
			TSharedPtr<FExtender> MainMenuExtender = MakeShareable(new FExtender);
			MainMenuExtender->AddMenuExtension(
#if (ENGINE_MAJOR_VERSION == 5)
				FName(TEXT("Tools")),
#else
				FName(TEXT("General")),
#endif
				EExtensionHook::After, 
				Commands,
				FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
				{
					MenuBuilder.AddMenuEntry(
						FBlueprintCopilotCommands::Get().OpenBlueprintCopilotWindow,
						NAME_None,
						FText::FromString(TEXT("BlueprintCopilot")),
						FText::FromString(TEXT("Opens BlueprintCopilot Window")),
						FSlateIcon(FBlueprintCopilotStyle::GetStyleSetName(), "BlueprintCopilot.MenuIcon")
					);
				})
			);

			// Extend Editors menu with the created Menu Extender.
			LevelEditor->GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
		}
	}
}

void FBlueprintCopilotModule::AddReferencedObjects(FReferenceCollector& Collector)
{
	// Prevent Editor Object from being garbage collected.
	if (Editor)
	{
		Collector.AddReferencedObject(Editor);
	}
}

#if (ENGINE_MAJOR_VERSION == 5)
FString FBlueprintCopilotModule::GetReferencerName() const
{
	return TEXT("BlueprintCopilotModuleGCObject");
}
#endif

bool FBlueprintCopilotModule::CanSpawnEditor()
{
	if (Editor)
	{
		UE_LOG(BlueprintCopilot2, Warning, TEXT("Call to Can Spawn Editor"));
		if (Editor->CanCreateEditorUI())
		{
			UE_LOG(BlueprintCopilot2, Warning, TEXT("Call to Can Spawn Editor2"));
			return true;
		}
	}
// Editor can be spawned only when the Editor object say that UI can be created.
	// if (Editor && Editor->CanCreateEditorUI())
	// {
	// 	return true;
	// }
			UE_LOG(BlueprintCopilot2, Warning, TEXT("Call to Cantttttttttt Spawn Editor"));
	return false;
}

TSharedRef<SDockTab> FBlueprintCopilotModule::SpawnEditor(const FSpawnTabArgs& Args)
{	
	// Spawn the Editor only when we can.
	if (CanSpawnEditor())
	{
		// Spawn new DockTab and fill it with newly created editor UI.
		TSharedRef<SDockTab> NewTab = SAssignNew(EditorTab, SDockTab)
			.TabRole(ETabRole::NomadTab)
			[
				Editor->CreateEditorUI()
			];

		// Tell the Editor Object about newly spawned DockTab, as it will 
		// need it to handle various editor actions.
		Editor->SetEditorTab(NewTab);

		// Return the DockTab to the Global Tab Manager.
		return NewTab;
	}

	// If editor can't be spawned - create an empty tab.
	return SAssignNew(EditorTab, SDockTab).TabRole(ETabRole::NomadTab);
}

bool FBlueprintCopilotModule::IsEditorSpawned()
{
	// Checks if the editor tab is already existing in the editor
	return FGlobalTabmanager::Get()->FindExistingLiveTab(BlueprintCopilotTabId).IsValid();
}

void FBlueprintCopilotModule::InvokeEditorSpawn()
{
	// Tries to invoke opening a plugin tab
		UE_LOG(BlueprintCopilot2, Warning, TEXT("Invoke Editor Spawn"));
	FGlobalTabmanager::Get()->TryInvokeTab(BlueprintCopilotTabId);
}
