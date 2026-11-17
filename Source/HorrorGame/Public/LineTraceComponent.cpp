// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceComponent.h"

#include "HorrorGamePlayerController.h"
#include "InteractableInterface.h"
#include "Kismet/GameplayStatics.h"

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
	
	PC = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

// Called every frame
void ULineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!ensureMsgf(PC, TEXT("Failed To Get Player Controller In Line Trace Component"))) return;
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector CameraLocation;
	FRotator CameraRotation;
		
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * LineTraceRange);
		
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel2, CollisionQueryParams);
			
	PC->ToggleCrosshair(bHit);
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