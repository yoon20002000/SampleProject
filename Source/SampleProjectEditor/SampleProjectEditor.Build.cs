
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SampleProjectEditor : ModuleRules
	{
		public SampleProjectEditor(ReadOnlyTargetRules Target) : base(Target)
		{

			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
			CppStandard = CppStandardVersion.Latest;

			PublicIncludePaths.AddRange(
			new string[] {
				"SampleProject/TPS",
				"SampleProjectEditor",
				}
			);


			PrivateIncludePathModuleNames.AddRange(
				new string[]
				{
					"Settings",
					"AssetTools",
					"MovieSceneTools",
					"ContentBrowser",

					"GameProjectGeneration",
					"MainFrame",
				});

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"ApplicationCore",
					"Engine",
					"Slate",
					"UMG",
					"EditorStyle",
					"UnrealEd",
					"RHI",
					"EditorScriptingUtilities",
				});

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"SlateCore",
					"RenderCore",
					"PropertyEditor",
					"MaterialEditor",
					"LevelEditor",
					"EditorStyle",
					"MovieScene",
					"MovieSceneTools",
					"Sequencer",
					"BlueprintGraph",       // AnimNodeBoneEditor
                    "Persona",              // AnimNodeBoneEditor
                    "MovieSceneTracks",     // AmEventParamScalar
                    "AnimGraph",            // AnimGraph
                    "ComponentVisualizers",
					"LevelSequence",
					"ActorPickerMode",
					"WorldBrowser",
					"AppFramework",
					"DesktopPlatform",
					"UMGEditor",
					"ClassViewer",
					"Projects",
					"AudioMixer",
					"SourceControl",
					"CinematicCamera",
					"ProceduralMeshComponent",
					"AssetRegistry",
					"AnimationBlueprintLibrary",
					"TimeManagement",
					"DeveloperSettings",
					"Json",
					"JsonUtilities",
				});

		}
	}
}
