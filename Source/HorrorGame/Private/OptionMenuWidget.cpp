// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenuWidget.h"
#include "ControlMenuWidget.h"
#include "GameplayMenuWidget.h"
#include "SettingMenuStates.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void UOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UserSettings = GEngine->GetGameUserSettings();
	checkf(UserSettings, TEXT("Failed To Get User Settings In Option Widget"));
	 
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (ensureAlwaysMsgf(Player, TEXT("Failed To Get Player Character In Option Menu WIdget")))
	{
		CameraComp = Player->FindComponentByClass<UCameraComponent>();
		checkf(CameraComp, TEXT("Failed To Get Camera Component In Option Widget"));
	}
	
	ControlMenuButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::ControlMenu);
	AudioMenuButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::AudioMenu);
	GameplayMenuButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::GameplayMenu);
		
	//Chromatic Abberation
	GameplayMenuUserCreated->ChromaticAbberationCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleChromaticAbberation);
		
	//VHS
	GameplayMenuUserCreated->VHSCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleVHS);
		
	//VSync
	GameplayMenuUserCreated->VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleVSync);
		
	//Fullscreen
	GameplayMenuUserCreated->FullscreenCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleFullscreen);
		
	//Film Grain
	GameplayMenuUserCreated->FilmGrainCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleFilmGrain);
		
	//Motion Blur
	GameplayMenuUserCreated->MotionBlurCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionMenuWidget::ToggleMotionBlur);
	
	BackButton->OnClicked.AddDynamic(this, &UOptionMenuWidget::BackMenu);
	
	ControlMenuUserCreated->MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UOptionMenuWidget::SetMouseSensitivity);
	
	if (AudioMenuUserCreated)
	{
		//Put The On Value Change For Audio
	}
	
	if (GameplayMenuUserCreated)
	{
		//Put The Gameplay Stuff Here
	}
}

void UOptionMenuWidget::ControlMenu() const
{
	MenuSwitch->SetActiveWidgetIndex(0);
}

void UOptionMenuWidget::AudioMenu() const
{
	MenuSwitch->SetActiveWidgetIndex(1);
}

void UOptionMenuWidget::GameplayMenu() const
{
	MenuSwitch->SetActiveWidgetIndex(2);
}

void UOptionMenuWidget::BackMenu() const
{
	OnToggleOptionWidget.Broadcast(ESettingMenus::BackMenu);
}

void UOptionMenuWidget::SetMouseSensitivity(const float NewMouseSens) const
{
	FNumberFormattingOptions NumberFormattingOptions;
	NumberFormattingOptions.MinimumFractionalDigits = 2;
	NumberFormattingOptions.MaximumFractionalDigits = 2;
	
	const FText MouseSensValue = FText::AsNumber(NewMouseSens, &NumberFormattingOptions);
	ControlMenuUserCreated->MouseSensitivityValue->SetText(MouseSensValue);
	
	OnChangedSensValue.Broadcast(NewMouseSens);
}

void UOptionMenuWidget::ToggleChromaticAbberation(const bool OnChecked) const
{
	if (!GameplayMenuUserCreated->ParameterCollection) return;
	
	const float CA_Toggle = OnChecked ? 1.0f : 0.0f;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GameplayMenuUserCreated->ParameterCollection, "CA_Toggle", CA_Toggle);
}

void UOptionMenuWidget::ToggleVHS(const bool OnChecked) const
{
	if (!GameplayMenuUserCreated->ParameterCollection) return;
	
	const float VHS_Toggle = OnChecked ? 1.0f : 0.0f;
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), GameplayMenuUserCreated->ParameterCollection, "VHS_Toggle", VHS_Toggle);
}

void UOptionMenuWidget::ToggleVSync(const bool OnChecked) const
{
	UserSettings->SetVSyncEnabled(OnChecked);
}

void UOptionMenuWidget::ToggleFullscreen(const bool OnChecked) const
{
	if (!OnChecked)
	{
		UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	else
	{
		UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	}
}

void UOptionMenuWidget::ToggleFilmGrain(const bool OnChecked) const
{
	CameraComp->PostProcessSettings.FilmGrainIntensity = OnChecked ? 1.0f : 0.0f;
}

void UOptionMenuWidget::ToggleMotionBlur(const bool OnChecked) const
{
	CameraComp->PostProcessSettings.MotionBlurAmount = OnChecked ? 0.5f : 0.0f;
}
