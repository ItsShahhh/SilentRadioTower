// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBase.h"

#include "QuestComponent.h"


// Sets default values
ADoorBase::ADoorBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	QuestComponent = CreateDefaultSubobject<UQuestComponent>("QuestComponent");
}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentDoorRot = FMath::RInterpTo(CurrentDoorRot, TargetDoorRot, DeltaTime, DoorRotSpeed);
	SetActorRotation(CurrentDoorRot);
		
	if (CurrentDoorRot.Equals(TargetDoorRot))
	{
		SetActorTickEnabled(false);
	}
}

void ADoorBase::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;
	
	OpenDoor(InteractActor);
}

void ADoorBase::OpenDoor(const AActor* InteractActor)
{
	//Only For Door That Required Quest To Open
	if (QuestComponent->QuestType == EQuestType::RequiredQuest)
	{
		if (!QuestComponent->RequiredQuest())
		{
			return;
		}
	}
	
	CurrentDoorRot = GetActorRotation();
	
	if (!bDoorOpen)
	{
		const float Dot = FVector::DotProduct(GetActorForwardVector(), InteractActor->GetActorForwardVector());
		const float YawRot = Dot >= 0.0f ? -180.0f : 180.0f;
		TargetDoorRot = FRotator(0.0f, YawRot, 0.0f);
		
		bDoorOpen = true;
	}
	else
	{
		TargetDoorRot = FRotator(0.0f, 0.0f, 0.0f);
		
		bDoorOpen = false;
	}
	
	SetActorTickEnabled(true);
}