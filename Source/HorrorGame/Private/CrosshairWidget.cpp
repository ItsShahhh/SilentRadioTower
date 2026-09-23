// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

#include "DialogueWidget.h"
#include "PauseMenuWidget.h"
#include "WorldSystem.h"

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnUpdateCrosshair.AddDynamic(this, &UCrosshairWidget::UpdateCrosshair);
		WorldSystem->OnToggleSettingWidget.AddDynamic(this, &UCrosshairWidget::ToggleWidget);
	}
}

void UCrosshairWidget::ToggleWidget(const ESettingMenus CurrentSettingMenus)
{
	switch (CurrentSettingMenus)
	{
		case PauseMenu:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
		
		case Resume:
		SetVisibility(ESlateVisibility::Visible);
		break;
		
		default:
		break;
	}
}

void UCrosshairWidget::UpdateCrosshair(const bool bIsInteracting) const
{
	if (bIsInteracting)
	{
		InnerCircle->SetOpacity(1.0f);
	}
	else
	{
		InnerCircle->SetOpacity(0.0f);
	}
}


