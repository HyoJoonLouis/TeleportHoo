// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class TeleportHoo : ModuleRules
{
	public TeleportHoo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"UMG", 
			"MotionTrajectory", 
			"Slate", 
			"SlateCore", 
			"OnlineSubsystem", 
			"OnlineSubsystemSteam", 
			"Niagara",
			"MovieScene", 
			"LevelSequence"
		});

        // Steam SDK 경로 설정
        string SteamVersion = "Steamv157";
        string EnginePath = Path.GetFullPath(Target.RelativeEnginePath);
        string SteamPath = Path.Combine(EnginePath, "Source", "ThirdParty", "Steamworks", SteamVersion, "sdk");

        PublicAdditionalLibraries.Add(Path.Combine(SteamPath, "redistributable_bin", "win64", "steam_api64.lib"));

        PublicIncludePaths.Add(Path.Combine(SteamPath, "public"));
    }
}
