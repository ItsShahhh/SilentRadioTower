// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"

#include "WorldSystem.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnToggleSettingWidget.AddDynamic(this, &UPauseMenuWidget::ToggleWidget);
	}
	
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickResumeButton);
	}
	
	if (OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickOptionsButton);
	}
	
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnClickMainMenuButton);
	}
}

void UPauseMenuWidget::ToggleWidget(const ESettingMenus CurrentSettingMenus)
{
	switch (CurrentSettingMenus)
	{
		case PauseMenu:
		case BackMenu:
		SetVisibility(ESlateVisibility::Visible);
		
		if (APlayerController* PC = GetOwningPlayer())
		{
			FInputModeUIOnly InputMode;
        
			// TakeWidget() grabs the underlying C++ Slate code and forces the lock
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        
			PC->SetInputMode(InputMode);
			PC->SetShowMouseCursor(true);
		}

		// 3. Force the physical widget to accept keyboard/mouse focus
		SetFocus();
		
		break;
		
		case Resume:
		case Options:
		SetVisibility(ESlateVisibility::Collapsed);
		break;
		
		default:
		break;
	}
}

void UPauseMenuWidget::OnClickResumeButton()
{
	if (!IsValid(WorldSystem)) return;
	
	SettingMenus = Resume;
	WorldSystem->OnToggleSettingWidget.Broadcast(SettingMenus);
	
	FReply::Handled().ClearUserFocus();
	FSlateApplication::Get().SetAllUserFocusToGameViewport();
	
	OnSwitchToGameMode.Broadcast();
}

void UPauseMenuWidget::OnClickOptionsButton()
{
	if (!IsValid(WorldSystem)) return;
	
	SettingMenus = Options;
	WorldSystem->OnToggleSettingWidget.Broadcast(SettingMenus);
}

void UPauseMenuWidget::OnClickMainMenuButton()
{
	SettingMenus = MainMenu;
}
