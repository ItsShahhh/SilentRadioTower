// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubSystem.h"
#include "GameplayTagContainer.h"
#include "GameSettings.h"

void UQuestSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//Get Game Settings From Project Setting For Data Table
	GameSettings =  GetDefault<UGameSettings>();
	if (!ensureAlwaysMsgf(GameSettings, TEXT("Failed To Get Game Setting In Quest SubSystem"))) return;
	
	QuestTable = GameSettings->GlobalQuestTable.LoadSynchronous();
	if (!ensureAlwaysMsgf(QuestTable, TEXT("Failed To Get Quest Table In Quest SubSystem"))) return;
}

void UQuestSubSystem::StartQuest(const FGameplayTag QuestTag)
{
	if (!ensureAlwaysMsgf(QuestTag.IsValid(), TEXT("Failed To Valid Completed Quest Tag In Quest SubSystem"))) return;
	
	ActiveQuest = QuestTable->FindRow<FQuestData>(QuestTag.GetTagName(), TEXT("Current Quest"), true);
	if (!ensureMsgf(ActiveQuest, TEXT("Failed To Get Active Quest In Quest SubSystem"))) return;
	
	OnUpdateQuest.Broadcast(*ActiveQuest);
}

void UQuestSubSystem::CompletedQuest(const FGameplayTag QuestTag)
{
	if (!ensureAlwaysMsgf(QuestTag.IsValid(), TEXT("Failed To Valid Completed Quest Tag In Quest SubSystem"))) return;
	if (!ensureAlwaysMsgf(ActiveQuest->NextQuest.IsValid(), TEXT("Forgot To Set Next Quest Tag In Quest Table"))) return;
	
	//Broadcast To Jumpscare Trigger To Check If This Specific Tag Able To Load It
	OnCheckJumpscare.Broadcast(QuestTag);
	
	FinishedQuestTag.AddTag(QuestTag);
	StartQuest(ActiveQuest->NextQuest);
}
