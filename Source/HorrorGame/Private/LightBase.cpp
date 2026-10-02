// Fill out your copyright notice in the Description page of Project Settings.


#include "LightBase.h"

#include "RandomEvent.h"
#include "WorldSystem.h"
#include "Components/SpotLightComponent.h"

// Sets default values
ALightBase::ALightBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(SceneComp);
	
	LightComp = CreateDefaultSubobject<USpotLightComponent>("LightComponent");
	LightComp->SetupAttachment(SceneComp);
}

// Called when the game starts or when spawned
void ALightBase::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnUpdateLight.AddDynamic(this, &ALightBase::ToggleLight);
		WorldSystem->OnUpdateRandomEvent.AddDynamic(this, &ALightBase::RandEventPower);
	}
}

// Called every frame
void ALightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALightBase::ToggleLight(const bool ToggleLight) const
{
	if (!IsValid(LightComp)) return;
	LightComp->SetVisibility(ToggleLight);
}

void ALightBase::RandEventPower(const ERandomEvent SelRandEvent) const
{
	if (SelRandEvent == ERandomEvent::Generator)
	{
		LightComp->SetVisibility(false);
	}
}

