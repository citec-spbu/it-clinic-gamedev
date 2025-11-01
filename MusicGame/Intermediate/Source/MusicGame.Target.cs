using UnrealBuildTool;

public class MusicGameTarget : TargetRules
{
	public MusicGameTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		Type = TargetType.Game;
		ExtraModuleNames.Add("MusicGame");
	}
}
