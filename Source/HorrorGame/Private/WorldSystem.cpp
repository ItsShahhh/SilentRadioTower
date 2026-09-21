// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSystem.h"

#include "DialogueData.h"
#include "GameSettings.h"
#include "QuestData.h"

void UWorldSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//Get Game Settings From Project Setting For Data Table
	GameSettings =  GetDefault<UGameSettings>();
	
	if (GameSettings)
	{
		//Only Load The Global Table Once
		LoadDialogueTable = GameSettings->GlobalDialogueTable.LoadSynchronous();
		
		//Only Load The Global Table Once
		LoadQuestTable = GameSettings->GlobalQuestTable.LoadSynchronous();
	}
}

void UWorldSystem::PlayDialogueData(const FGameplayTag DialogueTag) const
{
	if (!DialogueTag.IsValid()) return;
	
	if (LoadDialogueTable)
	{
		//Convert Dialogue Tag To FName To Use For Finding Row
		const FName& RowName = DialogueTag.GetTagName();
	
		//Find Row Based On Given Name
		//Also Check If Found Dialogue Is Valid
		if (const FDialogueLine* FoundDialogueLine = LoadDialogueTable->FindRow<FDialogueLine>(RowName, TEXT("Current Dialogue"),true))
		{
			//When Found The Name That Match The Row, It Will Be Broadcast To Dialogue Widget
			OnProcessDialogue.Broadcast(*FoundDialogueLine);
		}
	}
}

void UWorldSystem::StartQuest(const FGameplayTag QuestTag)
{
	if (!QuestTag.IsValid()) return;
	
	if (LoadQuestTable)
	{
		const FName& RowName = QuestTag.GetTagName();
	
		FoundQuestData = LoadQuestTable->FindRow<FQuestData>(RowName, TEXT("Current Quest"),true);
	
		if (FoundQuestData)
		{
			ActiveQuest.ActiveQuestTag = FoundQuestData->QuestTag;
			ActiveQuest.NextActiveQuest = FoundQuestData->NextQuest;
			
			OnUpdateQuest.Broadcast(*FoundQuestData);
		}
	}
}

void UWorldSystem::CompletedQuest()
{
	if (!FoundQuestData) return;
	if (!ActiveQuest.ActiveQuestTag.IsValid())	return;
	
	//When Completed Quest It Will BroadCast an Event Dispacther To Check for World Base Event To Activate
	OnCompletedQuest.Broadcast(ActiveQuest.ActiveQuestTag);
	
	//Active Next Quest When Completed The Active Quest
	if (ActiveQuest.ActiveQuestTag.MatchesTag(FoundQuestData->QuestTag))
	{
		StartQuest(ActiveQuest.NextActiveQuest);
	}
}
