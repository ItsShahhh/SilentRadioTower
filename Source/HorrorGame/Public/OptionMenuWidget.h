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

class UGameplayMenuWidget;
class UAudioMenuWidget;
class UControlMenuWidget;
enum class ESettingMenus : uint8;
class UWorldSystem;

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
	TObjectPtr<UWorldSystem> WorldSystem;
	UPROPERTY()
	TObjectPtr<UGameUserSettings> UserSettings;
	
	UFUNCTION()
	void ToggleWidget(const ESettingMenus CurrentSettingMenus);
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
	
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UControlMenuWidget> ControlMenuUserCreated;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAudioMenuWidget> AudioMenuUserCreated;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameplayMenuWidget> GameplayMenuUserCreated;
	
};
