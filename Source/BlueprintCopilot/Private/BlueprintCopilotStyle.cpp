// Copyright (c) 2022 Ren Brauge. All rights reserved.

#include "BlueprintCopilotStyle.h"

#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

// Null declaration of static variable (for linker needs)
TSharedPtr<FSlateStyleSet> FBlueprintCopilotStyle::StyleInstance = nullptr;

void FBlueprintCopilotStyle::Initialize()
{
    if (StyleInstance.IsValid() == false)
    {
        StyleInstance = Create();
        FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
    }
}

void FBlueprintCopilotStyle::Shutdown()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
    ensure(StyleInstance.IsUnique());
    StyleInstance.Reset();
}

void FBlueprintCopilotStyle::ReloadTextures()
{
    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
    }
}

FName FBlueprintCopilotStyle::GetStyleSetName()
{
    static FName StyleSetName(TEXT("BlueprintCopilot"));
    return StyleSetName;
}

TSharedRef<FSlateStyleSet> FBlueprintCopilotStyle::Create()
{
    // Create a new Style Set with a content root set to Resources directory of the plugin.
    TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
    Style->SetContentRoot(IPluginManager::Get().FindPlugin("BlueprintCopilot")->GetBaseDir() / TEXT("Resources"));

    // Create a new Slate Image Brush, which is Icon16.png from Resources directory.
    FSlateImageBrush* Brush = new FSlateImageBrush(Style->RootToContentDir(TEXT("Icon16"), TEXT(".png")), {16.f, 16.f});

    // Add newly created Brush to the Style Set.
    Style->Set("BlueprintCopilot.MenuIcon", Brush);

    // Result is a Style Set with menu icon in it.
    return Style;
}
