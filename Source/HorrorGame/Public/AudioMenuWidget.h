// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "AudioMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UAudioMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MasterAudioSlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MusicAudioSlider;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MasterAudioText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MusicAudioText;
	
};
