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
			"Niagara"
		});
		
		// Steam SDK 경로 설정
		string SteamVersion = "Steamv157"; // 사용 중인 Steamworks SDK 버전
		string SteamPath = Path.Combine(Target.UEThirdPartySourceDirectory, "Steamworks", SteamVersion);

		PublicAdditionalLibraries.Add(Path.Combine(SteamPath, "sdk", "redistributable_bin", "win64", "steam_api64.lib"));

		PublicIncludePaths.Add(Path.Combine(SteamPath, "sdk", "public"));
	}
}
