// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ExtraFunctionality : ModuleRules
{
    public ExtraFunctionality(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));


        //PublicDependencyModuleNames.AddRange(new string[] {
        //});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "ApplicationCore",
            "Core",
            "CoreUObject",
            "Engine",
            "HTTP",
            "InputCore",
            "NetworkReplayStreaming",
            "PhysicsCore",
            "Slate",
            "SlateCore",
            "UMG",
        });
    }
}
