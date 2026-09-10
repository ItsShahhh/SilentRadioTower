// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorrorGameCharacter.h"

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
#include "QuestWidget.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	if (DialogueWidget)
	{
		DialogueWidget->AddToViewport();
		DialogueWidget->SetVisibility(ESlateVisibility::Collapsed);
		DialogueWidget->OnDisableDialogue.AddDynamic(this, &AHorrorGameCharacter::DisableDialogueMode);
	}
	
	if (QuestWidget)
	{
		QuestWidget->AddToViewport();
	}
	
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
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
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
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

void AHorrorGameCharacter::TalkToNPC_Implementation(const FName& BoneDirection, const ACharacter* CurrentCharacter)
{
	if (BoneDirection.IsNone() || !IsValid(CurrentCharacter)) return;
	
	//Using Current Character Mesh and Getting The Bone Location We Set It As The Target For Player Head Rotation
	//Let's Say If I Want Player To Look At The Hand, When Calling Talk To NPC I Put hand_l Bone Name
	const FVector TargetLookAt = CurrentCharacter->GetMesh()->GetBoneLocation(BoneDirection);
	
	TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(FirstPersonCameraComponent->GetComponentLocation(), TargetLookAt);
	GetLocalViewingPlayerController()->SetInputMode(UIMode);
	GetLocalViewingPlayerController()->bShowMouseCursor = true;
	bDialogueMode = true;
}

void AHorrorGameCharacter::DisableDialogueMode() const
{
	GetLocalViewingPlayerController()->SetInputMode(GameMode);
	GetLocalViewingPlayerController()->bShowMouseCursor = false;
}
