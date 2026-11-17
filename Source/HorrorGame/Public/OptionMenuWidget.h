// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "OptionMenuWidget.generated.h"

/**
 * 
 */

class UCameraComponent;
class UGameplayMenuWidget;
class UAudioMenuWidget;
class UControlMenuWidget;
enum class ESettingMenus : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggleOptionWidget, ESettingMenus, CurrentSettingMenus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedSensValue, float, NewSensValue);


UCLASS()
class HORRORGAME_API UOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> MenuSwitch;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ControlMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AudioMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GameplayMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ControlMenuText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AudioMenuText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GameplayMenuText;

private:
	UPROPERTY()
	TObjectPtr<UGameUserSettings> UserSettings;
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComp;
	
	UFUNCTION()
	void ControlMenu() const;
	UFUNCTION()
	void AudioMenu() const;
	UFUNCTION()
	void GameplayMenu() const;
	UFUNCTION()
	void BackMenu() const;
	UFUNCTION()
	void SetMouseSensitivity(const float NewMouseSens) const;
	UFUNCTION()
	void ToggleChromaticAbberation(const bool OnChecked) const;
	UFUNCTION()
	void ToggleVHS(const bool OnChecked) const;
	UFUNCTION()
	void ToggleVSync(const bool OnChecked) const;
	UFUNCTION()
	void ToggleFullscreen(const bool OnChecked) const;
	UFUNCTION()
	void ToggleFilmGrain(const bool OnChecked) const;
	UFUNCTION()
	void ToggleMotionBlur(const bool OnChecked) const;
	
public:
	FOnToggleOptionWidget OnToggleOptionWidget;
	FOnChangedSensValue OnChangedSensValue;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UControlMenuWidget> ControlMenuUserCreated;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAudioMenuWidget> AudioMenuUserCreated;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameplayMenuWidget> GameplayMenuUserCreated;
};
