// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "PlayerMode.h"
#include "WorldSystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsInteract)
	{
		CurrentNPCRotation = UKismetMathLibrary::RInterpTo(CurrentNPCRotation, TargetNPCRotation, DeltaTime, NPCRotateSpeed);
		SetActorRotation(FRotator(0.0f, CurrentNPCRotation.Yaw, 0.0f));
	}
	
	if (CurrentNPCRotation.Equals(TargetNPCRotation, 0.1))
	{
		bIsInteract = false;
		SetActorTickEnabled(false);
	}
}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPCBase::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;
	
	//Enable Event Tick For Enemy
	SetActorTickEnabled(true);
	
	CurrentNPCRotation = GetActorRotation();
	TargetNPCRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InteractActor->GetActorLocation());
	bIsInteract = true;
	const FVector LookDirection = GetMesh()->GetBoneLocation(FName("head"));
	
	if (WorldSystem)
	{
		WorldSystem->PlayDialogueData(DialogueTable, NPCTag);
	}
	
	if (InteractActor->Implements<UPlayerMode>())
	{
		IPlayerMode::Execute_TalkToNPC(InteractActor, LookDirection);
	}
}


