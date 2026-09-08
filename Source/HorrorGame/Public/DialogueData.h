#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueData.generated.h"

USTRUCT(BlueprintType)
struct FDialogueLine : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText SpeakerName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FString> DialogueText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TextSpeed = 0.05f;
};
