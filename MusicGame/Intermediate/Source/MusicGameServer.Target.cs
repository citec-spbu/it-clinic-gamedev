using UnrealBuildTool;

public class MusicGameServerTarget : TargetRules
{
	public MusicGameServerTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		Type = TargetType.Server;
		ExtraModuleNames.Add("MusicGame");
	}
}
