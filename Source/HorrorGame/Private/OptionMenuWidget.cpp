// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenuWidget.h"

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
	
	if (MouseSensitivitySlider)
	{
		MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::OnSensitivityChanged);
	}
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::BackToMenu);
	}
}

void UOptionMenuWidget::ToggleWidget(const ESettingMenus CurrentSettingMenus)
{
	switch (CurrentSettingMenus)
	{
		case Options:
		SetVisibility(ESlateVisibility::Visible);
		break;
		
		case BackMenu:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
		
		default:
		break;
	}
}

void UOptionMenuWidget::OnSensitivityChanged(const float NewSensitivityValue) const
{
	OnUpdateSensitivity.Broadcast(NewSensitivityValue);
}

void UOptionMenuWidget::BackToMenu()
{
	if (!IsValid(WorldSystem)) return;
	
	WorldSystem->OnToggleSettingWidget.Broadcast(BackMenu);
	
	SetVisibility(ESlateVisibility::Collapsed);
	FReply::Handled().ClearUserFocus();
}