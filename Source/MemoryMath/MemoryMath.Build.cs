// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MemoryMath : ModuleRules
{
    public MemoryMath(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",          // ✅ UI sistemleri için
			"Slate",        // ✅ Gelişmiş UI bileşenleri için
			"SlateCore"     // ✅ Slate'in temel mantığı için
		});

        PrivateDependencyModuleNames.AddRange(new string[] {
			// Gerekirse özel modüller buraya eklenebilir
		});

        // Eğer OnlineSubsystem kullanacaksan aktif edebilirsin
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // Steam eklentisi kullanmak istersen .uproject içinde "Enabled": true olarak eklenmeli
    }
}