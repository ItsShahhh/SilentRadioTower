// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "CompQuestItem.generated.h"

class UQuestSubSystem;
/**
 * 
 */
UCLASS()
class ACompQuestItem : public AInteractableBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CompletedQuestTag;
	
private:
	UPROPERTY()
	TObjectPtr<UQuestSubSystem>QuestSubSystem;
	
public:
	virtual void Interactable_Implementation(AActor* InteractActor) override;
};
