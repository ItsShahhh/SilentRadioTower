#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "GameSettings.generated.h"

enum class ERandomEvent : uint8;

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "World Subsystem Settings"))
class HORRORGAME_API UGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// This will appear in the Project Settings menu for you to assign
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Global Data")
	TSoftObjectPtr<UDataTable> GlobalQuestTable;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Global Data")
	TSoftObjectPtr<UDataTable> GlobalDialogueTable;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Global Data")
	TMap<int32, ERandomEvent> RandomEventsMap;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Global Data")
	float RandEventTimer = 120.0f;
	
};