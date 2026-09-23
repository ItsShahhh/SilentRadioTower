#pragma once
#include "CoreMinimal.h"
#include "SettingMenuStates.generated.h"

UENUM()
enum class ESettingMenus : uint8
{
	PauseMenu UMETA(DisplayName = "Pause Menu"),
	Resume UMETA(DisplayName = "Resume"),
	Options UMETA(DisplayName = "Options"),
	MainMenu UMETA(DisplayName = "Main Menu"),
	BackMenu UMETA(DisplayName = "Back Menu"),
	Dialogue UMETA(DisplayName = "Dialogue"),
};
