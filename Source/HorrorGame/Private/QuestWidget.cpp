// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"

#include "PauseMenuWidget.h"
#include "QuestData.h"
#include "QuestSubSystem.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	QuestSubSystem = GetGameInstance()->GetSubsystem<UQuestSubSystem>();
	
	if (ensureMsgf(QuestSubSystem, TEXT("Failed To Get Quest SubSystem In Quest Widget")))
	{
		QuestSubSystem->OnUpdateQuest.AddDynamic(this, &UQuestWidget::InitializeQuest);
	}
	
}

void UQuestWidget::InitializeQuest(const FQuestData& QuestData) const
{
	QuestName->SetText(QuestData.QuestName);
	QuestDescription->SetText(QuestData.QuestDescription);
}