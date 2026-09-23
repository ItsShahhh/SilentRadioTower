// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CrosshairWidget.generated.h"

enum ESettingMenus : int;
class UWorldSystem;
class ULineTraceComponent;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OuterCircle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> InnerCircle;
	
private:
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UFUNCTION()
	void UpdateCrosshair(bool bIsInteracting) const;
	
	UFUNCTION()
	void ToggleWidget(const ESettingMenus CurrentSettingMenus);
	
};
