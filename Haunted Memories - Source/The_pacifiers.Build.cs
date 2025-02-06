// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class The_pacifiers : ModuleRules
{
	public The_pacifiers(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
