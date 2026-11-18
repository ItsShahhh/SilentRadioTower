// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleInteractables.h"

#include "HorrorGamePlayerController.h"
#include "WorldSystem.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

APuzzleInteractables::APuzzleInteractables()
{
	MainCamera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	MainCamera->SetupAttachment(RootComponent);
}

void APuzzleInteractables::BeginPlay()
{
	Super::BeginPlay();
	
	PC = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
}

void APuzzleInteractables::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;
	if (!ensureAlwaysMsgf(PC, TEXT("Failed To Get Player Controller In Interactable Base"))) return;
	if (!ensureAlwaysMsgf(WorldSystem, TEXT("Failed To Get World System In Interactable Base"))) return;
	
	if (!CachedActor)
	{
		CachedActor = InteractActor;
	}
	
	CachedActor->SetActorHiddenInGame(true);
	
	PC->InteractingMode(this, BlendSpeed);
	
	GetWorldTimerManager().SetTimer(ObjectiveTimerHandle, [this]
	{
		WorldSystem->InitiateRandomEvent();
	},
	1.0f,
	false
	);
}

void APuzzleInteractables::ExitFromInteractable()
{
	CachedActor->SetActorHiddenInGame(false);
	
	PC->UnInteractingMode(this, BlendSpeed);
}
