// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "OptionMenuWidget.generated.h"

/**
 * 
 */

enum ESettingMenus : int;
class UWorldSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateSensitivity, float, NewSensitivityValue);

UCLASS()
class HORRORGAME_API UOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MouseSensitivityText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FullscreenText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MainAudioText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SoundEffectAudioText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MusicAudioText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BackButtonText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MouseSensitivitySlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MusicAudioSlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SoundEffectAudioSlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MainAudioSlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FullscreenButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
	
private:
	UFUNCTION()
	void OnSensitivityChanged(const float NewSensitivityValue) const;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UFUNCTION()
	void ToggleWidget(const ESettingMenus CurrentSettingMenus);
	
	UFUNCTION()
	void BackToMenu();
	
public:
	FOnUpdateSensitivity OnUpdateSensitivity;
	
};
