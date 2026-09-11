// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestData.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSystem.generated.h"

class UGameSettings;
struct FQuestData;
struct FDialogueLine;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateLight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableLight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCrosshair, bool, bIsInteracting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnableDialogue, const FDialogueLine&, DialogueData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateQuest, const FQuestData&, QuestData);
/**
 * 
 */
UCLASS()
class HORRORGAME_API UWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	FQuestData* FoundQuestData;
	
	UPROPERTY()
	const UGameSettings* GameSettings;
	
	UPROPERTY()
	TObjectPtr<UDataTable> LoadQuestTable;
	
	UPROPERTY()
	TObjectPtr<UDataTable> LoadDialogueTable;
	
public:
	FOnUpdateLight OnUpdateLight;
	FOnEnableDialogue OnProcessDialogue;
	FOnDisableLight OnDisableLight;
	FOnUpdateQuest OnUpdateQuest;
	FOnUpdateCrosshair OnUpdateCrosshair;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayDialogueData(const FGameplayTag DialogueTag);
	UFUNCTION(BlueprintCallable)
	void StartQuest(const FGameplayTag QuestTag);
	void ReportQuest() const;
};
