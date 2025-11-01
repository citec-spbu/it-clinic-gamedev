using UnrealBuildTool;

public class MusicGameClientTarget : TargetRules
{
	public MusicGameClientTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		Type = TargetType.Client;
		ExtraModuleNames.Add("MusicGame");
	}
}
