// Copyright Epic Games, Inc. All Rights Reserved.


#include "HorrorGamePlayerController.h"
#include "CrosshairWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "HorrorGameCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "HorrorGame.h"
#include "OptionMenuWidget.h"
#include "PauseMenuWidget.h"
#include "SettingMenuStates.h"
#include "Widgets/Input/SVirtualJoystick.h"

AHorrorGamePlayerController::AHorrorGamePlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AHorrorGameCameraManager::StaticClass();
}

void AHorrorGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (ensureAlwaysMsgf(PauseMenuClass, TEXT("Forgot To Put Pause Menu Class In Player Controller")))
	{
		auto* PauseMenu = CreateWidget<UPauseMenuWidget>(this, PauseMenuClass);
		if (ensureAlwaysMsgf(PauseMenu, TEXT("Failed To Get Pause Menu Reference in Player Controller")))
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
			CachedPauseMenu = PauseMenu;
			CachedPauseMenu->OnTogglePauseMenu.AddDynamic(this, &AHorrorGamePlayerController::ToggleWidget);
		}
	}
	
	if (ensureAlwaysMsgf(OptionMenuClass, TEXT("Failed To Get Option Menu Class In Player Controller")))
	{
		auto* OptionWidget = CreateWidget<UOptionMenuWidget>(this, OptionMenuClass);
		if (ensureAlwaysMsgf(OptionWidget, TEXT("Failed To Get Option Menu Reference in Player Controller")))
		{
			OptionWidget->AddToViewport();
			OptionWidget->SetVisibility(ESlateVisibility::Collapsed);
			CachedOptionMenu = OptionWidget;
			CachedOptionMenu->OnToggleOptionWidget.AddDynamic(this, &AHorrorGamePlayerController::ToggleWidget);
			CachedOptionMenu->OnChangedSensValue.AddDynamic(this, &AHorrorGamePlayerController::ChangedSensValue);
		}
	}
	
	if (ensureAlwaysMsgf(CrosshairClass, TEXT("Forgot To Put Crosshair Class In Player Controller")))
	{
		auto* CrosshairWidget = CreateWidget<UCrosshairWidget>(this, CrosshairClass);
		if (ensureAlwaysMsgf(CrosshairWidget, TEXT("Failed To Get Crosshair Widget Reference in Player Controller")))
		{
			CrosshairWidget->AddToViewport();
			CachedCrosshairWidget = CrosshairWidget;
		}
	}
	
	// only spawn touch controls on local player controllers
	if (IsLocalPlayerController() && ShouldUseTouchControls())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogHorrorGame, Error, TEXT("Could not spawn mobile controls widget."));

		}
	}
}

void AHorrorGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ensureAlwaysMsgf(TogglePause, TEXT("TogglePause is null")))
		{
			EnhancedInputComp->BindAction(TogglePause, ETriggerEvent::Started, this, &AHorrorGamePlayerController::TogglePauseMenu);
		}
	}

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}

bool AHorrorGamePlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void AHorrorGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	MyPawn = InPawn;
}

void AHorrorGamePlayerController::ToggleCrosshair(const bool bToggleCrosshair) const
{
	CachedCrosshairWidget->UpdateCrosshair(bToggleCrosshair);
}

void AHorrorGamePlayerController::ChangedSensValue(const float NewSensValue)
{
	FinalSensValue = NewSensValue;
}

void AHorrorGamePlayerController::TogglePauseMenu()
{
	if (!bPauseMenu)
	{
		UIModes();
		bPauseMenu = true;
	}
	else
	{
		GameModes();
		bPauseMenu = false;
	}
}

void AHorrorGamePlayerController::ToggleWidget(const ESettingMenus CurrentMenus)
{
	switch (CurrentMenus)
	{
	case ESettingMenus::Resume:
		CachedPauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		CachedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		GameModes();
		bPauseMenu = false;
		break;
		
	case ESettingMenus::Options:
		CachedPauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		CachedOptionMenu->SetVisibility(ESlateVisibility::Visible);
		break;
		
	case ESettingMenus::BackMenu:
		CachedPauseMenu->SetVisibility(ESlateVisibility::Visible);
		CachedOptionMenu->SetVisibility(ESlateVisibility::Collapsed);
		break;
		
	default:
		break;
	}
}

void AHorrorGamePlayerController::InteractingMode(AActor* InteractingActor, const float Blendspeed)
{
	if (!ensureAlwaysMsgf(InteractingActor, TEXT("Failed To Get Interacting Actor In Player Controller"))) return;
	if (!ensureAlwaysMsgf(MyPawn, TEXT("Failed To Get Player Pawn In Player Controller"))) return;
	
	//Change Camera View
	SetViewTargetWithBlend(InteractingActor, Blendspeed, VTBlend_Linear);
	
	//SWAP INPUT (Freeze Player, Wake Up Machine)
	MyPawn->DisableInput(this);
	InteractingActor->EnableInput(this);
	
	CachedCrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AHorrorGamePlayerController::UnInteractingMode(AActor* InteractingActor, const float Blendspeed)
{
	if (!ensureAlwaysMsgf(InteractingActor, TEXT("Failed To Get Interacting Actor In Player Controller"))) return;
	if (!ensureAlwaysMsgf(MyPawn, TEXT("Failed To Get Player Pawn In Player Controller"))) return;
	
	SetViewTargetWithBlend(MyPawn, Blendspeed, VTBlend_Linear);
	
	// Fixed Only Can Turn Camera Halfway Because Of Unlocking Mouse
	// Lock the invisible mouse back to the center of the screen!
	const FInputModeGameOnly GameOnlyMode;
	SetInputMode(GameOnlyMode);
	
	bShowMouseCursor = false;
	
	InteractingActor->DisableInput(this);
	MyPawn->EnableInput(this);
	
	CachedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
}

void AHorrorGamePlayerController::UIModes()
{
	CachedPauseMenu->SetVisibility(ESlateVisibility::Visible);
	CachedCrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
		
	SetInputMode(UIMode);
	bShowMouseCursor = true;
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
}

void AHorrorGamePlayerController::GameModes()
{
	CachedPauseMenu->SetVisibility(ESlateVisibility::Collapsed);
	CachedOptionMenu->SetVisibility(ESlateVisibility::Collapsed);
	CachedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		
	SetInputMode(GameMode);
	bShowMouseCursor = false;
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}
