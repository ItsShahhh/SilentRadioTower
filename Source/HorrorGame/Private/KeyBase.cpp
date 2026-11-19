// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyBase.h"

#include "QuestComponent.h"

// Sets default values
AKeyBase::AKeyBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
	checkf(QuestComponent, TEXT("Failed To Get Quest Component In Key Base"));
}

// Called when the game starts or when spawned
void AKeyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKeyBase::Interactable_Implementation(AActor* InteractActor)
{
	PickupKey();
}

void AKeyBase::PickupKey()
{
	QuestComponent->CompletedQuest();
	Destroy();
}
