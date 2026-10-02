// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestData.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSystem.generated.h"

enum class ERandomEvent : uint8;
class UGameSettings;
struct FQuestData;
struct FDialogueLine;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateLight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableLight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchGameMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSwitchUIMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleSettingWidget, const ESettingMenus, SettingEnum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateRandomEvent, const ERandomEvent, RandomEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompletedQuest, FGameplayTag, ActiveQuestTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCrosshair, bool, bIsInteracting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnableDialogue, const FDialogueLine&, DialogueData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateQuest, const FQuestData&, QuestData);
/**
 * 
 */

USTRUCT()
struct FActiveQuest
{
	GENERATED_BODY()
	
	UPROPERTY()
	FGameplayTag ActiveQuestTag;
	
	UPROPERTY()
	FGameplayTag NextActiveQuest;
};

UCLASS()
class HORRORGAME_API UWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	FQuestData* FoundQuestData;
	FActiveQuest ActiveQuest;
	
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
	FOnCompletedQuest OnCompletedQuest;
	FOnToggleSettingWidget OnToggleSettingWidget;
	FOnSwitchGameMode OnSwitchGameMode;
	FOnSwitchUIMode OnSwitchUIMode;
	FOnUpdateRandomEvent OnUpdateRandomEvent;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void PlayDialogueData(const FGameplayTag DialogueTag) const;
	UFUNCTION(BlueprintCallable)
	void StartQuest(const FGameplayTag QuestTag);
	void CompletedQuest();
	void InitiateRandomEvent() const;
};
