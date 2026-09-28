// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenuWidget.h"
#include "ControlMenuWidget.h"
#include "SettingMenuStates.h"
#include "PauseMenuWidget.h"
#include "WorldSystem.h"

void UOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnToggleSettingWidget.AddDynamic(this, &UOptionMenuWidget::ToggleWidget);
	}
	
	if (ControlMenuButton)
	{
		ControlMenuButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::ControlMenu);
	}
	
	if (AudioMenuButton)
	{
		AudioMenuButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::AudioMenu);
	}
	
	if (GameplayMenuButton)
	{
		GameplayMenuButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::GameplayMenu);
	}
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::BackMenu);
	}
	
	if (ControlMenuUserCreated)
	{
		ControlMenuUserCreated->MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::SetMouseSensitivity);
	}
	
	if (AudioMenuUserCreated)
	{
		//Put The On Value Change For Audio
	}
	
	if (GameplayMenuUserCreated)
	{
		//Put The Gameplay Stuff Here
	}
}

void UOptionMenuWidget::ToggleWidget(const ESettingMenus CurrentSettingMenus)
{
	switch (CurrentSettingMenus)
	{
		case ESettingMenus::Options:
		SetVisibility(ESlateVisibility::Visible);
		break;
		
		case ESettingMenus::BackMenu:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
		
		default:
		break;
	}
}

void UOptionMenuWidget::ControlMenu() const
{
	if (!MenuSwitch) return;
	MenuSwitch->SetActiveWidgetIndex(0);
}

void UOptionMenuWidget::AudioMenu() const
{
	if (!MenuSwitch) return;
	MenuSwitch->SetActiveWidgetIndex(1);
}

void UOptionMenuWidget::GameplayMenu() const
{
	if (!MenuSwitch) return;
	MenuSwitch->SetActiveWidgetIndex(2);
}

void UOptionMenuWidget::BackMenu() const
{
	if (!IsValid(WorldSystem)) return;
	WorldSystem->OnToggleSettingWidget.Broadcast(ESettingMenus::BackMenu);
}

void UOptionMenuWidget::SetMouseSensitivity(const float NewMouseSens) const
{
	if (!ControlMenuUserCreated) return;
	ControlMenuUserCreated->OnUpdateNewSens.Broadcast(NewMouseSens);
	
	FNumberFormattingOptions NumberFormattingOptions;
	NumberFormattingOptions.MinimumFractionalDigits = 2;
	NumberFormattingOptions.MaximumFractionalDigits = 2;
	
	const FText MouseSensValue = FText::AsNumber(NewMouseSens, &NumberFormattingOptions);
	ControlMenuUserCreated->MouseSensitivityValue->SetText(MouseSensValue);
}
