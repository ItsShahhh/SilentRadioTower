// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "ControlMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UControlMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MouseSensitivityText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MouseSensitivityValue;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MouseSensitivitySlider;
	
};
