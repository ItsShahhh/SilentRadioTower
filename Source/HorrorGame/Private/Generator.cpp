// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "WorldSystem.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGenerator::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(WorldSystem)) return;
	WorldSystem->OnUpdateLight.Broadcast();
}

