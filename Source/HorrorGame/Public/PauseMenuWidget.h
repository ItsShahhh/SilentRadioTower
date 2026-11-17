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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTogglePauseMenu, ESettingMenus, CurrentSettingMenus);

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
	UFUNCTION()
	void OnClickResumeButton() const;
	UFUNCTION()
	void OnClickOptionsButton() const;
	UFUNCTION()
	void OnClickMainMenuButton() const;
	
public:
	FOnTogglePauseMenu OnTogglePauseMenu;
};
