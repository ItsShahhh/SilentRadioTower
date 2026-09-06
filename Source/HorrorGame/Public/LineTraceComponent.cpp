// Fill out your copyright notice in the Description page of Project Settings.


#include "LineTraceComponent.h"

#include "InteractableInterface.h"
#include "Camera/CameraComponent.h"

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

	// ...
	
}

// Called every frame
void ULineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULineTraceComponent::StartTraceLine(const UCameraComponent* CameraComp)
{
	const FVector StartLocation = CameraComp->GetComponentLocation();
	const FVector ForwardVector = CameraComp->GetForwardVector();
	const FVector EndLocation = StartLocation + (ForwardVector * LineTraceRange);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	
	bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel12,CollisionQueryParams);
	
	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!IsValid(HitActor)) return;
		
		if (!HitActor->Implements<UInteractableInterface>()) return;
		IInteractableInterface::Execute_Interactable(HitActor, GetOwner());
	}
}