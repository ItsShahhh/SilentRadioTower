#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Questdata.generated.h"

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText QuestDescription;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bQuestCompleted;
};
