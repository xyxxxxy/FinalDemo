// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FinalDemo : ModuleRules
{
	public FinalDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject", "Engine", "InputCore", "HeadMountedDisplay","AIModule","GameplayTasks","UMG","GameplayTags" });
	}
}
