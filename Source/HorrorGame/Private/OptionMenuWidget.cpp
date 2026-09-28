// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenuWidget.h"
#include "ControlMenuWidget.h"
#include "GameplayMenuWidget.h"
#include "SettingMenuStates.h"
#include "WorldSystem.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/GameUserSettings.h"

void UOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	UserSettings = GEngine->GetGameUserSettings();
	
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
		GameplayMenuUserCreated->ChromaticAbberationCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleChromaticAbberation);
		GameplayMenuUserCreated->VHSCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleVHS);
		GameplayMenuUserCreated->VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleVSync);
		GameplayMenuUserCreated->FullscreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleFullscreen);
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

void UOptionMenuWidget::ToggleChromaticAbberation(const bool OnChecked) const
{
	if (!IsValid(GameplayMenuUserCreated)) return;
	if (!GameplayMenuUserCreated->ParameterCollection) return;
	
	const float CA_Toggle = OnChecked ? 1.0f : 0.0f;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GameplayMenuUserCreated->ParameterCollection, "CA_Toggle", CA_Toggle);
}

void UOptionMenuWidget::ToggleVHS(const bool OnChecked) const
{
	if (!IsValid(GameplayMenuUserCreated)) return;
	if (!GameplayMenuUserCreated->ParameterCollection) return;
	
	const float VHS_Toggle = OnChecked ? 1.0f : 0.0f;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GameplayMenuUserCreated->ParameterCollection, "VHS_Toggle", VHS_Toggle);
}

void UOptionMenuWidget::ToggleVSync(const bool OnChecked) const
{
	if (!UserSettings) return;
	UserSettings->SetVSyncEnabled(OnChecked);
}

void UOptionMenuWidget::ToggleFullscreen(const bool OnChecked) const
{
	if (!UserSettings) return;
	
	if (!OnChecked)
	{
		UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	else
	{
		UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	}
}
