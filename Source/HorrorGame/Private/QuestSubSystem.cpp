// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSubSystem.h"
#include "GameplayTagContainer.h"
#include "GameSettings.h"

void UQuestSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//Get Game Settings From Project Setting For Data Table
	GameSettings =  GetDefault<UGameSettings>();
	QuestTable = GameSettings->GlobalQuestTable;
}

void UQuestSubSystem::StartQuest(const FGameplayTag QuestTag)
{
	if (!ensureAlwaysMsgf(QuestTag.IsValid(), TEXT("Failed To Valid Completed Quest Tag In Quest SubSystem"))) return;
	if (!ensureAlwaysMsgf(QuestTable, TEXT("Failed To Get Quest Table In Quest SubSystem"))) return;
	
	ActiveQuest = QuestTable->FindRow<FQuestData>(QuestTag.GetTagName(), TEXT("Current Quest"), true);
}

void UQuestSubSystem::CompletedQuest(const FGameplayTag QuestTag)
{
	if (!ensureAlwaysMsgf(QuestTag.IsValid(), TEXT("Failed To Valid Completed Quest Tag In Quest SubSystem"))) return;
	if (!ensureAlwaysMsgf(QuestTable, TEXT("Failed To Get Quest Table In Quest SubSystem"))) return;
	if (!ensureAlwaysMsgf(ActiveQuest, TEXT("Failed To Get Quest In Quest SubSystem"))) return;
	
	FinishedQuestTag.AddTag(QuestTag);
	
	StartQuest(ActiveQuest->NextQuest);
}
