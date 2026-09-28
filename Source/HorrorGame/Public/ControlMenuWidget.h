// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "ControlMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateNewSens, const float, NewMouseSensitivity);

class UWorldSystem;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UControlMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnUpdateNewSens OnUpdateNewSens;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MouseSensitivityText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MouseSensitivityValue;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MouseSensitivitySlider;
};
