// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class The_Park : ModuleRules
{
	public The_Park(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
