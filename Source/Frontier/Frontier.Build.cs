using UnrealBuildTool;
using System.IO;

public class Frontier : ModuleRules
{
	public Frontier(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"AIModule", 
			"Slate",
			"SlateCore",
			"UMG",
			"NavigationSystem",
		});

		CppStandard = CppStandardVersion.Latest;

		// Add the source root directory to the inclusion paths
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../Source/Frontier"));

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.Add("UnrealEd");
		}
	}
}
