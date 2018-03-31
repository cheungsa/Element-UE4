// Copyright 2017 Isaac Hsu.

using UnrealBuildTool;

public class InputBufferEditor : ModuleRules
{
	public InputBufferEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // ... add public include paths required here ...
        PublicIncludePaths.AddRange(new string[] {
			"InputBuffer/Public",
			"InputBufferEditor/Public",
		});				
		
		// ... add private include paths required here ...
		PrivateIncludePaths.AddRange(new string[] {
			"InputBuffer/Private",
			"InputBufferEditor/Private",
            "AssetTools/Public",
        });			
		
		// ... add public dependencies that you statically link with here ...
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"InputBuffer",
        });			
		
		// ... add private dependencies that you statically link with here ...	
		PrivateDependencyModuleNames.AddRange(new string[] {
			"CoreUObject",
			"Engine",
            "InputCore",
            "UnrealEd",
            "AssetTools",
        });
				
		// ... add any modules that your module loads dynamically here ...
		DynamicallyLoadedModuleNames.AddRange( new string[] {});
	}
}
