// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilotBase.h"

#include "BlueprintCopilotWidget.h"

#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1))
#include "AssetRegistry/AssetRegistryModule.h"
#else
#include "AssetRegistryModule.h"
#endif

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "LevelEditor.h"
DEFINE_LOG_CATEGORY(BlueprintCopilot)

void UBlueprintCopilotBase::Init()
{
    // Empty virtual function - to be overridden
}

void UBlueprintCopilotBase::InitializeTheWidget()
{
    // Empty virtual function - to be overridden
}

void UBlueprintCopilotBase::SetEditorTab(const TSharedRef<SDockTab>& NewEditorTab)
{
    EditorTab = NewEditorTab;
}

UEditorUtilityWidgetBlueprint* UBlueprintCopilotBase::GetUtilityWidgetBlueprint()
{
    // Get the Editor Utility Widget Blueprint from the content directory.
    FAssetRegistryModule& AssetRegistryModule =
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
#if ((ENGINE_MAJOR_VERSION == 5) && (ENGINE_MINOR_VERSION >= 1))
    FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(
        FSoftObjectPath("/BlueprintCopilot/BlueprintCopilotWidget_BP.BlueprintCopilotWidget_BP"));
#else
    FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(
        "/BlueprintCopilot/BlueprintCopilotWidget_BP.BlueprintCopilotWidget_BP");
#endif
    return Cast<UEditorUtilityWidgetBlueprint>(AssetData.GetAsset());
}

bool UBlueprintCopilotBase::CanCreateEditorUI()
{
    auto t = GetUtilityWidgetBlueprint() != nullptr;
    if (t)
    {
        UE_LOG(BlueprintCopilot, Warning, TEXT("Call to Can Create Editor UI"));
    }
    else
    {
        UE_LOG(BlueprintCopilot, Warning, TEXT("Call to Canttttttt Create Editor UI"));
    }

    return true;

    // Editor UI can be created only when we have proper Editor Utility Widget Blueprint available.
    // return GetUtilityWidgetBlueprint() != nullptr;
}

TSharedRef<SWidget> UBlueprintCopilotBase::CreateEditorUI()
{
    UE_LOG(BlueprintCopilot, Warning, TEXT("Call to Create Editor UI"));

    // Register OnMapChanged event so we can properly handle Tab and Widget when changing levels.
    FLevelEditorModule& LevelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    LevelEditor.OnMapChanged().AddUObject(this, &UBlueprintCopilotBase::ChangeTabWorld);

    // Create the Widget
    return CreateEditorWidget();
}

TSharedRef<SWidget> UBlueprintCopilotBase::CreateEditorWidget()
{
    UE_LOG(BlueprintCopilot, Warning, TEXT("Call to Create Editor Widget"));

    TSharedRef<SWidget> CreatedWidget = SNullWidget::NullWidget;
    if (UEditorUtilityWidgetBlueprint* UtilityWidgetBP = GetUtilityWidgetBlueprint())
    {
        // Create Widget from the Editor Utility Widget BP.
        CreatedWidget = UtilityWidgetBP->CreateUtilityWidget();

        // Save the pointer to the created Widget and initialize it.
        EditorWidget = Cast<UBlueprintCopilotWidget>(UtilityWidgetBP->GetCreatedWidget());
        if (EditorWidget)
        {
            InitializeTheWidget();
        }
    }

    // Returned Widget will be docked into the Editor Tab.
    return CreatedWidget;
}

void UBlueprintCopilotBase::ChangeTabWorld(UWorld* World, EMapChangeType MapChangeType)
{
    // Handle the event when editor map changes.
    UE_LOG(BlueprintCopilot, Warning, TEXT("Call to Change Tab World"));
    if (MapChangeType == EMapChangeType::TearDownWorld)
    {
        // If the world is destroyed - set the Tab content to null and null the Widget.
        if (EditorTab.IsValid())
        {
            EditorTab.Pin()->SetContent(SNullWidget::NullWidget);
        }
        if (EditorWidget)
        {
            EditorWidget->Rename(nullptr, GetTransientPackage());
            EditorWidget = nullptr;
        }
    }
    else if (MapChangeType == EMapChangeType::NewMap || MapChangeType == EMapChangeType::LoadMap)
    {
        // If the map has been created or loaded and the Tab is valid - put a new Widget into this Tab.
        if (EditorTab.IsValid())
        {
            EditorTab.Pin()->SetContent(CreateEditorWidget());
        }
    }
}
