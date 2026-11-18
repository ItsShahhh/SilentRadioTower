// Fill out your copyright notice in the Description page of Project Settings.


#include "CompQuestItem.h"
#include "QuestSubSystem.h"

void ACompQuestItem::BeginPlay()
{
	Super::BeginPlay();
	
	QuestSubSystem = GetGameInstance()->GetSubsystem<UQuestSubSystem>();
}

void ACompQuestItem::Interactable_Implementation(AActor* InteractActor)
{
	if (!ensureMsgf(QuestSubSystem, TEXT("Failed To Get Quest SubSystsem In Req Quest Item"))) return;
	if (!ensureMsgf(CompletedQuestTag.IsValid(), TEXT("Failed To Validate Completed Quest Tag In Comp Quest Item"))) return;
	
	QuestSubSystem->CompletedQuest(CompletedQuestTag);
	
	Destroyed();
}
