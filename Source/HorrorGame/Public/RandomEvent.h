#pragma once
#include "CoreMinimal.h"
#include "RandomEvent.generated.h"

UENUM(BlueprintType)
enum class ERandomEvent : uint8
{
	Satllite UMETA(DisplayName = "Satllite"),
	Generator UMETA(DisplayName = "Generator"),
	HidingJumpscare UMETA(DisplayName = "HidingJumpscare"),
};