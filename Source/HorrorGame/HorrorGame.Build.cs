// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;

public class HorrorGame : ModuleRules
{
	[Obsolete("Obsolete")]
	public HorrorGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Disable deprecation warnings temporarily
		bEnableUndefinedIdentifierWarnings = false;
		ShadowVariableWarningLevel = WarningLevel.Off;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayTags",
			"DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"HorrorGame",
			"HorrorGame/Variant_Horror",
			"HorrorGame/Variant_Horror/UI",
			"HorrorGame/Variant_Shooter",
			"HorrorGame/Variant_Shooter/AI",
			"HorrorGame/Variant_Shooter/UI",
			"HorrorGame/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
