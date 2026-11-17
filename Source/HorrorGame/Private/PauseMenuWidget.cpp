// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "OptionMenuWidget.h"
#include "SettingMenuStates.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickResumeButton);
	OptionButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickOptionsButton);
	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickMainMenuButton);
}

void UPauseMenuWidget::OnClickResumeButton() const
{
	OnTogglePauseMenu.Broadcast(ESettingMenus::Resume);
}

void UPauseMenuWidget::OnClickOptionsButton() const
{
	OnTogglePauseMenu.Broadcast(ESettingMenus::Options);
}

void UPauseMenuWidget::OnClickMainMenuButton() const
{
	//SettingMenus = MainMenu;
}