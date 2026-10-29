// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "WorldSystem.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainMesh");
	MainMesh->SetupAttachment(RootComp);
	
	MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	Pawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	checkf(Pawn, TEXT("Failed To Get Pawn In Interactable Base"));
	
	PC = Cast<APlayerController>(Pawn->GetController());
	
	checkf(PC, TEXT("Failed To Get Player Controller In Interactable Base"));
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	checkf(WorldSystem, TEXT("Failed To Get World System In Interactable Base"));
}

// Called every frame
void AInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableBase::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;
	
	InteractingActor = InteractActor;
	InteractingActor->SetActorHiddenInGame(true);
	
	//Change Camera
	PC->SetViewTargetWithBlend(this, BlendSpeed, VTBlend_Linear);

	//SWAP INPUT (Freeze Player, Wake Up Machine)
	Pawn->DisableInput(PC);
	this->EnableInput(PC);
	
	if (InteractActor->Implements<UInteractableInterface>())
	{
		Execute_ToggleCrosshair(InteractActor, false);
	}
	
	GetWorldTimerManager().SetTimer(ObjectiveTimerHandle, [this]
	{
		WorldSystem->InitiateRandomEvent();
	},
	1.0f,
	false
	);
}

void AInteractableBase::ExitFromInteractable()
{
	InteractingActor->SetActorHiddenInGame(false);
	
	// Fixed Only Can Turn Camera Halfway Because Of Unlocking Mouse
	// Lock the invisible mouse back to the center of the screen!
	const FInputModeGameOnly GameOnlyMode;
	PC->SetInputMode(GameOnlyMode);
						
	PC->bShowMouseCursor = false;
	
	PC->SetViewTargetWithBlend(Pawn, BlendSpeed, VTBlend_Linear);
	this->DisableInput(PC);
	Pawn->EnableInput(PC);
	
	if (InteractingActor->Implements<UInteractableInterface>())
	{
		Execute_ToggleCrosshair(InteractingActor, true);
	}
}

