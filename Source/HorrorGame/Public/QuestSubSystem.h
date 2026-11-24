// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubSystem.generated.h"

struct FQuestData;
class UGameSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateQuest, const FQuestData&, QuestData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckJumpscare, const FGameplayTag&, JumpscareTag);

/**
 * 
 */
UCLASS()
class HORRORGAME_API UQuestSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<const UGameSettings>GameSettings;
	UPROPERTY()
	TObjectPtr<UDataTable>QuestTable;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void StartQuest(const FGameplayTag QuestTag);
	void CompletedQuest(const FGameplayTag QuestTag);
	
	FOnUpdateQuest OnUpdateQuest;
	FOnCheckJumpscare OnCheckJumpscare;
	FGameplayTagContainer FinishedQuestTag;
	FQuestData* ActiveQuest;
};
