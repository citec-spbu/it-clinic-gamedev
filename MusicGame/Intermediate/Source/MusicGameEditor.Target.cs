using UnrealBuildTool;

public class MusicGameEditorTarget : TargetRules
{
	public MusicGameEditorTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		Type = TargetType.Editor;
		ExtraModuleNames.Add("MusicGame");
	}
}
