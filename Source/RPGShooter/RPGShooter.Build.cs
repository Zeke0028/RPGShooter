// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGShooter : ModuleRules
{
	public RPGShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "RPGShooter/Public", "RPGShooter/Public/Generic", "RPGShooter/Public/Gameplay" });

		PrivateIncludePaths.AddRange(new string[] { "RPGShooter/Private", "RPGShooter/Private/Debug" });

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"DataRegistry",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"GameplayAbilities", "GameplayTags", "GameplayTasks",
			"Slate", "SlateCore", "UMG",
			"OnlineSubsystem", "OnlineSubsystemUtils",
			"EnhancedInput",
			"NetCore",
		});

		DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemNull", "OnlineSubsystemEOS" });
	}
}
