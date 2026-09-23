// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"

#include "PauseMenuWidget.h"
#include "QuestData.h"
#include "SettingMenuStates.h"
#include "WorldSystem.h"

void UQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnUpdateQuest.AddDynamic(this, &UQuestWidget::InitializeQuest);
		WorldSystem->OnToggleSettingWidget.AddDynamic(this, &UQuestWidget::ToggleWidget);
	}
}

void UQuestWidget::ToggleWidget(const ESettingMenus CurrentSettingMenus) 
{
	switch (CurrentSettingMenus)
	{
		case ESettingMenus::PauseMenu:
		case ESettingMenus::BackMenu:
		SetVisibility(ESlateVisibility::Visible);
		break;
		
		case ESettingMenus::Options:
		case ESettingMenus::Resume:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
		
		default:
		break;
	}
}

void UQuestWidget::InitializeQuest(const FQuestData& QuestData) const
{
	if (QuestData.QuestName.IsEmpty() || QuestData.QuestDescription.IsEmpty()) return;
	
	QuestName->SetText(QuestData.QuestName);
	QuestDescription->SetText(QuestData.QuestDescription);
}