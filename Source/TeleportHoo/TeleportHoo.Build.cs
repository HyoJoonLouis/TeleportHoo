// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TeleportHoo : ModuleRules
{
	public TeleportHoo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "MotionTrajectory", "Slate", "SlateCore", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
