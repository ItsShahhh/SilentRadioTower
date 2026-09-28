// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorrorGameCharacter.h"
#include "ControlMenuWidget.h"
#include "CrosshairWidget.h"
#include "DialogueWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorGame.h"
#include "LineTraceComponent.h"
#include "OptionMenuWidget.h"
#include "PauseMenuWidget.h"
#include "QuestWidget.h"
#include "WorldSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMeshActor.h"
#include "SettingMenuStates.h"

AHorrorGameCharacter::AHorrorGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
	
	//Attach Components
	LineTraceComponent = CreateDefaultSubobject<ULineTraceComponent>("Line Trace Component");
}

void AHorrorGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnSwitchGameMode.AddDynamic(this, &AHorrorGameCharacter::GameModeState);
		WorldSystem->OnSwitchUIMode.AddDynamic(this, &AHorrorGameCharacter::UIModeState);
	}
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DialogueWidget = CreateWidget<UDialogueWidget>(PC, DialogueWidgetClass);
		
		if (DialogueWidget)
		{
			DialogueWidget->AddToViewport();
			DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		QuestWidget = CreateWidget<UQuestWidget>(PC, QuestWidgetClass);
		
		if (QuestWidget)
		{
			QuestWidget->AddToViewport();
			QuestWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		CrosshairWidget = CreateWidget<UCrosshairWidget>(PC, CrosshairWidgetClass);
		
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
		
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(PC, PauseMenuWidgetClass);
		
		if (PauseMenuWidget)
		{
			PauseMenuWidget->AddToViewport();
			PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		
		OptionMenuWidget = CreateWidget<UOptionMenuWidget>(PC, OptionMenuWidgetClass);
		
		if (OptionMenuWidget)
		{
			OptionMenuWidget->AddToViewport();
			OptionMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			OptionMenuWidget->ControlMenuUserCreated->OnUpdateNewSens.AddDynamic(this, &AHorrorGameCharacter::ChangeSensitivity);
		}
	}
}

void AHorrorGameCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bDialogueMode && GetController())
	{
		const FRotator CurrentControlRot = GetController()->GetControlRotation();
		const FRotator SmoothRotation = UKismetMathLibrary::RInterpTo(CurrentControlRot, TargetCameraRotation, DeltaSeconds, CameraSpeed);
		
		// Because bUsePawnControlRotation is true, the camera will perfectly follow this without any offset!
		GetController()->SetControlRotation(SmoothRotation);
		
		if (SmoothRotation.Equals(TargetCameraRotation, 0.1))
		{
			bDialogueMode = false;
		}
	}
}

void AHorrorGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AHorrorGameCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AHorrorGameCharacter::LookInput);
		
		//Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::Interact);
		
		//Toggle Pause Menu
		EnhancedInputComponent->BindAction(TogglePauseMenuAction, ETriggerEvent::Started, this, &AHorrorGameCharacter::TogglePauseMenu);
	}
	else
	{
		UE_LOG(LogHorrorGame, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AHorrorGameCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AHorrorGameCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AHorrorGameCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw * BasedSensitivity);
		AddControllerPitchInput(Pitch * BasedSensitivity);
	}
}

void AHorrorGameCharacter::ChangeSensitivity(const float NewSensValue)
{
	BasedSensitivity = NewSensValue;
}

void AHorrorGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AHorrorGameCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AHorrorGameCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHorrorGameCharacter::Interact()
{
	if (!IsValid(LineTraceComponent)) return;
	LineTraceComponent->CalledTraceLine();
}

void AHorrorGameCharacter::LookAtActor_Implementation(const AActor* LookAtActor)
{
	if (!IsValid(LookAtActor)) return;
	
	UIModeState();
	
	//Create Empty Vector
	FVector TargetLookAt;
	
	if (const ACharacter* Character = Cast<ACharacter>(LookAtActor))
	{
		//Give The NPC Head Player Bone Because That What Default Bone To Look At When Talking To NPC
		TargetLookAt = Character->GetMesh()->GetSocketLocation("head");
	}
	else if (const AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(LookAtActor))
	{
		TargetLookAt = StaticMeshActor->GetStaticMeshComponent()->GetComponentLocation();
	}
	
	TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(FirstPersonCameraComponent->GetComponentLocation(), TargetLookAt);
	bDialogueMode = true;
}

void AHorrorGameCharacter::ToggleCrosshair_Implementation(const bool bToggleCrosshair)
{
	if (!IsValid(CrosshairWidget)) return;
	
	if (bToggleCrosshair)
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHorrorGameCharacter::TogglePauseMenu()
{
	if (!IsValid(WorldSystem)) return;
	
	UIModeState();
	
	WorldSystem->OnToggleSettingWidget.Broadcast(ESettingMenus::PauseMenu);
}

void AHorrorGameCharacter::GameModeState() const
{
	if (!IsValid(GetLocalViewingPlayerController())) return;
	
	GetLocalViewingPlayerController()->SetInputMode(GameMode);
	GetLocalViewingPlayerController()->bShowMouseCursor = false;
	GetLocalViewingPlayerController()->SetIgnoreMoveInput(false);
	GetLocalViewingPlayerController()->SetIgnoreLookInput(false);
}

void AHorrorGameCharacter::UIModeState() const
{
	if (!IsValid(GetLocalViewingPlayerController())) return;
	
	GetLocalViewingPlayerController()->SetInputMode(UIMode);
	GetLocalViewingPlayerController()->bShowMouseCursor = true;
	GetLocalViewingPlayerController()->SetIgnoreMoveInput(true);
	GetLocalViewingPlayerController()->SetIgnoreLookInput(true);
}