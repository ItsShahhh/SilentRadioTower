// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"

#include "QuestData.h"
#include "WorldSystem.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnUpdateQuest.AddDynamic(this, &UQuestWidget::InitializeQuest);
	}
}

void UQuestWidget::InitializeQuest(const FQuestData& QuestData) const
{
	if (QuestData.QuestName.IsEmpty() || QuestData.QuestDescription.IsEmpty()) return;
	
	QuestName->SetText(QuestData.QuestName);
	QuestDescription->SetText(QuestData.QuestDescription);
}