// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "GameplayMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UGameplayMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> FullscreenCheckBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> ChromaticAbberationCheckBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> VHSCheckBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> VSyncCheckBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> FullscreenText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChromaticAbberationText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> VHSText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> VSyncText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialParameterCollection> ParameterCollection;
	
};
