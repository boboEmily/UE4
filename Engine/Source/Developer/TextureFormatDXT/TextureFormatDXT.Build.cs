// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TextureFormatDXT : ModuleRules
{
	public TextureFormatDXT(TargetInfo Target)
	{
		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"TargetPlatform",
				"TextureCompressor",
				"Engine"
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"ImageCore",
				"ImageWrapper"
			}
			);

		AddThirdPartyPrivateStaticDependencies(Target, "nvTextureTools");
	}
}
