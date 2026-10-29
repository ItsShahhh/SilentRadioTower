// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include  "Components/TextBlock.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */

enum class ESettingMenus : uint8;
class UWorldSystem;

UCLASS()
class HORRORGAME_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResumeButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OptionButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResumeText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OptionText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MainMenuText;
	
private:
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UFUNCTION()
	void OnClickResumeButton() const;
	UFUNCTION()
	void OnClickOptionsButton() const;
	UFUNCTION()
	void OnClickMainMenuButton() const;
	UFUNCTION()
	void ToggleWidget(const ESettingMenus CurrentSettingMenus);
	
};
