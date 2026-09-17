// Fill out your copyright notice in the Description page of Project Settings.


#include "MorseCode.h"
#include "Components/SpotLightComponent.h"

// Sets default values
AMorseCode::AMorseCode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MorseLight = CreateDefaultSubobject<USpotLightComponent>("MorseLight");
	MorseLight->SetupAttachment(MainMesh);
}

// Called when the game starts or when spawned
void AMorseCode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMorseCode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMorseCode::Interactable_Implementation(AActor* InteractActor)
{
	Super::Interactable_Implementation(InteractActor);
	
	if (!IsValid(MorseLight)) return;
	
	//Randomize Number For Each Interact
	const int32 RandNumber = FMath::RandRange(0,9);
	
	if (MorseSignalMap.IsEmpty() || !MorseSignalMap.Contains(RandNumber)) return;
	
	//Based On The Random Number It Chose The Morse Signal Map
	ChoosenMorseSignal = MorseSignalMap[RandNumber];
	
	if (!bIsMachineActive)
	{
		ActivateMorseLight();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("The Machine Is Already Active"));
	}
}

void AMorseCode::ActivateMorseLight()
{
	if (ChoosenMorseSignal.Signals.IsEmpty()) return;
	
	if (!ChoosenMorseSignal.Signals.IsValidIndex(NextSignal))
	{
		//If Next Signal Is More Than Array That Mean The Morse Code Is Only That Much
		//After That We Reset The Number For The Next Morse Code
		NextSignal = 0;
		bIsMachineActive = false;
		return;
	}
	
	//Set The bIsMachineActive To True Indicate Machine Is Current Active
	bIsMachineActive = true;

	//Crate Empty Float
	float BeepDuration = 0;
	
	//It Will Set The Beep Duration Based On The Signals That Been Chose By The Next Signal Index
	//Reminder: Can Just Used Ternary But Make It Like So It Easier To Understand 
	if (ChoosenMorseSignal.Signals[NextSignal] == EMorseSignal::Dot)
	{
		BeepDuration = ShortBeep;
	}
	else if (ChoosenMorseSignal.Signals[NextSignal] == EMorseSignal::Dash)
	{
		BeepDuration = LongBeep;
	}
	
	//Activate The Light
	MorseLight->SetVisibility(true);
	
	//Added Beep Sound Here
	
	GetWorldTimerManager().SetTimer(MorseLightTimerHandle, [this]
	{
		MorseLight->SetVisibility(false);
		NextSignal++;
		
		//Stop Beep Sound Here
		
		GetWorldTimerManager().SetTimer(MorseLightTimerHandle, [this]
		{
			ActivateMorseLight();
		},
		1.0f,
		false
		);
	},
	BeepDuration,
	false
	);
}


