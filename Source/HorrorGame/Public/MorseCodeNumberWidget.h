// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MorseCodeNumberWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UMorseCodeNumberWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CodeNum1;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CodeNum2;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CodeNum3;
	
	
};
