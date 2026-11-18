// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSystem.h"
#include "GameSettings.h"
#include "RandomEvent.h"

void UWorldSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	//Get Game Settings From Project Setting For Data Table
	GameSettings =  GetDefault<UGameSettings>();
	
	checkf(GameSettings, TEXT("Failed To Get GameSettings From World System"));
}

void UWorldSystem::InitiateRandomEvent()
{
	//This Bool Avoid The Random Event Get Called Twice
	if (!bAlreadyInitiated)
	{
		//Set Already Initiated True
		bAlreadyInitiated = true;
		
		FTimerHandle RandEventTimer;
		GetWorld()->GetTimerManager().SetTimer(RandEventTimer, [this]
		{
			const int32 TotalRandomEvent =  GameSettings->RandomEventsMap.Num();
			const int32 RandomEvent = FMath::RandRange(1, TotalRandomEvent);
		
			if (const ERandomEvent* SelRandEvent = GameSettings->RandomEventsMap.Find(RandomEvent))
			{
				OnUpdateRandomEvent.Broadcast(*SelRandEvent);
				
				//Set Already Initiated False
				bAlreadyInitiated = false;
			}
		},
		GameSettings->RandEventTimer,
		false
		);	
	}
}