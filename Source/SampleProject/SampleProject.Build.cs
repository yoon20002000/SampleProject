// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SampleProject : ModuleRules
{
	public SampleProject(ReadOnlyTargetRules Target) : base(Target)
	{
		bool bUseOnlineSubsystemV1 = true;
		PrivateDependencyModuleNames.AddRange(new string[] { });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.AddRange(
			new string[] {
				"SampleProject/TPS",
			}
		);
		PublicDefinitions.Add("COMMONUSER_OSSV1=" + (bUseOnlineSubsystemV1 ? "1" : "0"));
		
		
		PublicDependencyModuleNames.AddRange(new string[] { 
		"Core", 
		"CoreUObject", 
		"Engine", 
		"InputCore", 
		"EnhancedInput", 
		"GameplayTasks", 
		"UMG",
		"Slate", 
		"SlateCore",
		"CommonUI",
		"GameplayTags",
		"CommonInput",
		});

		if (bUseOnlineSubsystemV1)
		{
			PublicDependencyModuleNames.Add("OnlineSubsystem");
		}

		//PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
