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
}

void UWorldSystem::PlayDialogueData(const FGameplayTag DialogueTag)
{
	if (!GameSettings) return;
	if (!DialogueTag.IsValid()) return;
	
	if (!LoadDialogueTable)
	{
		//Only Load The Global Table Once
		LoadDialogueTable = GameSettings->GlobalDialogueTable.LoadSynchronous();
	}
	
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
	if (!GameSettings) return;
	if (!QuestTag.IsValid()) return;
	
	if (!LoadQuestTable)
	{
		//Only Load The Global Table Once
		LoadQuestTable = GameSettings->GlobalQuestTable.LoadSynchronous();
	}
	
	if (LoadQuestTable)
	{
		const FName& RowName = QuestTag.GetTagName();
	
		FoundQuestData = LoadQuestTable->FindRow<FQuestData>(RowName, TEXT("Current Quest"),true);
	
		if (FoundQuestData)
		{
			OnUpdateQuest.Broadcast(*FoundQuestData);
		}
	}
}

void UWorldSystem::ReportQuest() const
{
	if (!FoundQuestData) return;
	
	FoundQuestData->bQuestCompleted = true;
}
