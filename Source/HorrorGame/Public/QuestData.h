#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTags.h"
#include "Questdata.generated.h"

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag QuestTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag NextQuest;
};
