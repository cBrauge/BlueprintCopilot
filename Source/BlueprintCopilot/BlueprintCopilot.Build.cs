// Copyright (c) 2022 Ren Brauge. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class BlueprintCopilot : ModuleRules
{
  public BlueprintCopilot(ReadOnlyTargetRules Target) : base(Target)
  {
    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

    PrivateIncludePaths.Add("BlueprintCopilot/Private");

    PublicDependencyModuleNames.AddRange(
      new string[]
      {
        "Core",
        "InputCore"
      }
    );


    PrivateDependencyModuleNames.AddRange(
      new string[]
      {
        "Engine",
        "CoreUObject",
        "Slate",
        "SlateCore",
        "UnrealEd",
        "WorkspaceMenuStructure",
        "DesktopPlatform",
        "Blutility",
        "UMG",
        "UMGEditor",
        "EditorStyle",
        "Projects",
        "BlueprintGraph"
      }
    );

    CMakeTarget.add(Target, this, "LibGuidance", Path.Combine(this.ModuleDirectory, "../Deps/LibBlueprintCopilot"), "", true);
    CMakeTarget.add(Target, this, "libcurl", Path.Combine(this.ModuleDirectory, "../Deps/LibBlueprintCopilot"), "", true);
    // CMakeTarget.add(Target, this, "nlohmann_json", Path.Combine(this.ModuleDirectory, "../Deps/LibBlueprintCopilot"), "", true);
    CMakeTarget.add(Target, this, "oai", Path.Combine(this.ModuleDirectory, "../Deps/LibBlueprintCopilot"), "", true);
  }
}
