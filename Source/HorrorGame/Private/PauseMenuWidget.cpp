// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "WorldSystem.h"
#include "SettingMenuStates.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	checkf(WorldSystem, TEXT("Failed To Get World System In Pause Menu Widget"));
	
	WorldSystem->OnToggleSettingWidget.AddDynamic(this, &UPauseMenuWidget::ToggleWidget);
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickResumeButton);
	OptionButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickOptionsButton);
	MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickMainMenuButton);
}

void UPauseMenuWidget::ToggleWidget(const ESettingMenus CurrentSettingMenus)
{
	switch (CurrentSettingMenus)
	{
		case ESettingMenus::PauseMenu:
		case ESettingMenus::BackMenu:
		SetVisibility(ESlateVisibility::Visible);
		break;
		
		case ESettingMenus::Resume:
		case ESettingMenus::Options:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
		
		default:
		break;
	}
}

void UPauseMenuWidget::OnClickResumeButton() const
{
	WorldSystem->OnToggleSettingWidget.Broadcast(ESettingMenus::Resume);
	WorldSystem->OnSwitchGameMode.Broadcast();
}

void UPauseMenuWidget::OnClickOptionsButton() const
{
	WorldSystem->OnToggleSettingWidget.Broadcast(ESettingMenus::Options);
}

void UPauseMenuWidget::OnClickMainMenuButton() const
{
	//SettingMenus = MainMenu;
}
