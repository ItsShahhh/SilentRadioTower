// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceComponent.h"
#include "InteractableInterface.h"
#include "WorldSystem.h"

// Sets default values for this component's properties
ULineTraceComponent::ULineTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULineTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	Pawn = Cast<APawn>(GetOwner());
	
	if (Pawn)
	{
		PC = Pawn->GetLocalViewingPlayerController();
	}
	
	check(PC);
	check(Pawn);
	check(WorldSystem);
}

// Called every frame
void ULineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector CameraLocation;
	FRotator CameraRotation;
		
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * LineTraceRange);
		
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel2, CollisionQueryParams);
			
	WorldSystem->OnUpdateCrosshair.Broadcast(bHit);
}

void ULineTraceComponent::CalledTraceLine() const
{
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor)) return;
		
		if (!HitActor->Implements<UInteractableInterface>()) return;
		IInteractableInterface::Execute_Interactable(HitActor, GetOwner());
	}
}