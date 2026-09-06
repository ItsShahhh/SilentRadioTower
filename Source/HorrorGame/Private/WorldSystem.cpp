// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSystem.h"

#include "DialogueData.h"

void UWorldSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldSystem::PlayDialogueData(const UDataTable* DialogueTable,  const FGameplayTag DialogueTag) const
{
	if (!DialogueTable || !DialogueTag.IsValid()) return;
	
	//Convert Dialogue Tag To FName To Use For Finding Row
	const FName& RowName = DialogueTag.GetTagName();
	
	//Find Row Based On Given Name
	//Also Check If Found Dialogue Is Valid
	if (const FDialogueLine* FoundDialogueLine = DialogueTable->FindRow<FDialogueLine>(RowName, TEXT("Current Dialogue"),true))
	{
		//When Found The Name That Match The Row, It Will Be Broadcast To Dialogue Widget
		OnProcessDialogue.Broadcast(*FoundDialogueLine);
	}
}
