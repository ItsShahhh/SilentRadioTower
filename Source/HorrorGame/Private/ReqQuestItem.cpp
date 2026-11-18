// Fill out your copyright notice in the Description page of Project Settings.


#include "ReqQuestItem.h"
#include "QuestSubSystem.h"

void AReqQuestItem::BeginPlay()
{
	Super::BeginPlay();
	
	QuestSubSystem = GetGameInstance()->GetSubsystem<UQuestSubSystem>();
}

void AReqQuestItem::Interactable_Implementation(AActor* InteractActor)
{
	if (!ensureMsgf(QuestSubSystem, TEXT("Failed To Get Quest SubSystsem In Req Quest Item"))) return;
	if (!ensureMsgf(RequiredQuestTag.IsValid(), TEXT("Failed To Validate Required Quest Tag In Req Quest Item"))) return;
	
	if (QuestSubSystem->FinishedQuestTag.HasTag(RequiredQuestTag))
	{
		//Do Animation Unlock
		
		Destroyed();
	}
}