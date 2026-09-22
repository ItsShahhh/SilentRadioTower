// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickResumeButton);
	}
	
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickOptionsButton);
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickMainMenuButton);
	}
}

void UPauseMenuWidget::OnClickResumeButton()
{
	SettingMenus = Resume;
	OnUpdateClickButton.Broadcast(SettingMenus);
}

void UPauseMenuWidget::OnClickOptionsButton()
{
	SettingMenus = Options;
	OnUpdateClickButton.Broadcast(SettingMenus);
}

void UPauseMenuWidget::OnClickMainMenuButton()
{
	SettingMenus = MainMenu;
	OnUpdateClickButton.Broadcast(SettingMenus);
}
