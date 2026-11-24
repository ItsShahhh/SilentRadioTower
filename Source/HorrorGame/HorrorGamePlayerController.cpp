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
#include "QuestSubSystem.h"
#include "QuestWidget.h"
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
	
	if (ensureAlwaysMsgf(PauseClass, TEXT("Forgot To Put Pause Menu Class In Player Controller")))
	{
		CachedPauseWidget = CreateWidget<UPauseMenuWidget>(this, PauseClass);
		if (ensureAlwaysMsgf(CachedPauseWidget, TEXT("Failed To Get Pause Menu Reference in Player Controller")))
		{
			CachedPauseWidget->AddToViewport();
			CachedPauseWidget->SetVisibility(ESlateVisibility::Collapsed);
			CachedPauseWidget->OnTogglePauseMenu.AddDynamic(this, &AHorrorGamePlayerController::ToggleWidget);
		}
	}
	
	if (ensureAlwaysMsgf(CrosshairClass, TEXT("Forgot To Put Crosshair Class In Player Controller")))
	{
		CachedCrosshairWidget = CreateWidget<UCrosshairWidget>(this, CrosshairClass);
		if (ensureAlwaysMsgf(CachedCrosshairWidget, TEXT("Failed To Get Crosshair Widget Reference in Player Controller")))
		{
			CachedCrosshairWidget->AddToViewport();
		}
	}
	
	if (ensureAlwaysMsgf(QuestClass, TEXT("Forgot To Put Quest Class In Player Controller")))
	{
		CachedQuestWidget = CreateWidget<UQuestWidget>(this, QuestClass);
		if (ensureAlwaysMsgf(CachedQuestWidget, TEXT("Failed To Get Quest Widget Reference in Player Controller")))
		{
			CachedQuestWidget->AddToViewport();
			CachedQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	if (UQuestSubSystem* QuestSubSystem = GetGameInstance()->GetSubsystem<UQuestSubSystem>())
	{
		const FGameplayTag StartQuest = FGameplayTag::RequestGameplayTag(FName("Quest.Test"));
		
		if (StartQuest.IsValid())
		{
			QuestSubSystem->StartQuest(StartQuest);
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
		LoadOption();
		bPauseMenu = true;
	}
	else
	{
		GameModes();
		UnloadOption();
		bPauseMenu = false;
	}
}

void AHorrorGamePlayerController::ToggleWidget(const ESettingMenus CurrentMenus)
{
	switch (CurrentMenus)
	{
	case ESettingMenus::Resume:
		CachedPauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		CachedQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
		CachedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		GameModes();
		UnloadOption();
		bPauseMenu = false;
		break;
		
	case ESettingMenus::Options:
		CachedPauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		CachedQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
		CachedOptionWidget->SetVisibility(ESlateVisibility::Visible);
		break;
		
	case ESettingMenus::BackMenu:
		CachedPauseWidget->SetVisibility(ESlateVisibility::Visible);
		CachedQuestWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		CachedOptionWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	CachedPauseWidget->SetVisibility(ESlateVisibility::Visible);
	CachedQuestWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	CachedCrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
		
	SetInputMode(UIMode);
	bShowMouseCursor = true;
	SetIgnoreMoveInput(true);
	SetIgnoreLookInput(true);
}

void AHorrorGamePlayerController::GameModes()
{
	CachedPauseWidget->SetVisibility(ESlateVisibility::Collapsed);
	CachedQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
	CachedOptionWidget->SetVisibility(ESlateVisibility::Collapsed);
	CachedCrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		
	SetInputMode(GameMode);
	bShowMouseCursor = false;
	SetIgnoreMoveInput(false);
	SetIgnoreLookInput(false);
}

void AHorrorGamePlayerController::LoadOption()
{
	if (bAlreadyLoaded)
	{
		GetWorldTimerManager().ClearTimer(UnloadTimerHandle);
		return;
	}
	
	CachedOptionWidget = CreateWidget<UOptionMenuWidget>(this, OptionClass.LoadSynchronous());
	if (!ensureAlwaysMsgf(CachedOptionWidget, TEXT("Failed To Get Option Menu Reference in Player Controller"))) return;
	
	bAlreadyLoaded = true;
	
	CachedOptionWidget->AddToViewport();
	CachedOptionWidget->SetVisibility(ESlateVisibility::Collapsed);
	CachedOptionWidget->OnToggleOptionWidget.AddDynamic(this, &AHorrorGamePlayerController::ToggleWidget);
	CachedOptionWidget->OnChangedSensValue.AddDynamic(this, &AHorrorGamePlayerController::ChangedSensValue);
}

void AHorrorGamePlayerController::UnloadOption()
{
	GetWorldTimerManager().SetTimer(UnloadTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]
	{
		CachedOptionWidget->RemoveFromParent();
		CachedOptionWidget->OnToggleOptionWidget.RemoveDynamic(this, &AHorrorGamePlayerController::ToggleWidget);
		CachedOptionWidget->OnChangedSensValue.RemoveDynamic(this, &AHorrorGamePlayerController::ChangedSensValue);
		CachedOptionWidget = nullptr;
		bAlreadyLoaded = false;
	}
	),
	UnloadTimer,
	false);
}
