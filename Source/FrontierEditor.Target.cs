using UnrealBuildTool;
using System.Collections.Generic;

public class FrontierEditorTarget : TargetRules
{
	public FrontierEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Frontier");
	}
}
