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
}

// Called every frame
void ULineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Inside your Line Trace Component
	if (Pawn)
	{
		if (PC)
		{
			FVector CameraLocation;
			FRotator CameraRotation;
		
			PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

			const FVector TraceStart = CameraLocation;
			const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * LineTraceRange);
		
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(GetOwner());
	
			bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel12,CollisionQueryParams);
			
			if (bLastHit)
			{
				if (bHit)
				{
					WorldSystem->OnUpdateCrosshair.Broadcast(bHit);
				}
				else
				{
					WorldSystem->OnUpdateCrosshair.Broadcast(bHit);
				}
			}
			//It Will Check Down Here and If bLastHit Still False It Will Not Call Function Above
			//But If bLastHit Is True When Its Checking Down Here It Will Call The Function Above
			//Reminder: This Is To Make Sure It Only Called Event Dispacther Once When bHit Is False
			bLastHit = bHit;
		}
	}
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