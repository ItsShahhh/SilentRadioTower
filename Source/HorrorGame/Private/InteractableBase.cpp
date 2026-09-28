// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "WorldSystem.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>("MainMesh");
	MainMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (!WorldSystem)
	{
		WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	}
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
	
	if (!Pawn && !PC)
	{
		Pawn = Cast<APawn>(InteractActor);
		if (Pawn)
		{
			PC = Cast<APlayerController>(Pawn->GetController());
		}
	}
    
	if (!IsValid(PC)) return;

	//Change Camera
	PC->SetViewTargetWithBlend(this, BlendSpeed, VTBlend_Linear);

	//SWAP INPUT (Freeze Player, Wake Up Machine)
	Pawn->DisableInput(PC);
	this->EnableInput(PC);
	
	if (InteractActor->Implements<UInteractableInterface>())
	{
		Execute_ToggleCrosshair(InteractActor, false);
	}
}

void AInteractableBase::ExitFromInteractable()
{
	if (!InteractingActor) return;
	
	InteractingActor->SetActorHiddenInGame(false);
	
	PC->SetViewTargetWithBlend(Pawn, BlendSpeed, VTBlend_Linear);
	this->DisableInput(PC);
	Pawn->EnableInput(PC);
	
	if (InteractingActor->Implements<UInteractableInterface>())
	{
		Execute_ToggleCrosshair(InteractingActor, true);
	}
}

