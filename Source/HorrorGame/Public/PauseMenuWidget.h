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

UENUM()
enum ESettingMenus
{
	Resume UMETA(DisplayName = "Resume"),
	Options UMETA(DisplayName = "Options"),
	MainMenu UMETA(DisplayName = "Main Menu"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateClickButton, const ESettingMenus, CurrentSettingMenus);

UCLASS()
class HORRORGAME_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
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
	
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnClickResumeButton();
	UFUNCTION()
	void OnClickOptionsButton();
	UFUNCTION()
	void OnClickMainMenuButton();
	
	ESettingMenus SettingMenus;
	
public:
	FOnUpdateClickButton OnUpdateClickButton;
	
};
