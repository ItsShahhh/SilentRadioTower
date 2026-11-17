// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableBase.h"
#include "HorrorGamePlayerController.h"
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
	
	PC = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
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
	if (!ensureAlwaysMsgf(PC, TEXT("Failed To Get Player Controller In Interactable Base"))) return;
	
	InteractingActor = InteractActor;
	InteractingActor->SetActorHiddenInGame(true);
	
	PC->InteractingMode(this, BlendSpeed);
	
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
	
	PC->UnInteractingMode(this, BlendSpeed);
}

