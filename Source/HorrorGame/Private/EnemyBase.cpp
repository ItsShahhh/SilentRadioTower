// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "AIController.h"
#include "RandomEvent.h"
#include "WorldSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
	
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	AIController = Cast<AAIController>(GetController());
	
	if (IsValid(AIController))
	{
		BBComp = AIController->GetBlackboardComponent(); 
	}
	
	if (WorldSystem)
	{
		WorldSystem->OnUpdateRandomEvent.AddDynamic(this, &AEnemyBase::MoveToHidingSpot);
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsValid(Player))
	{
		const FVector EnemLoc = GetActorLocation();
		const FVector PlayerLoc = Player->GetActorLocation();
			
		const FVector DirArrow = ( EnemLoc - PlayerLoc ).GetSafeNormal(); 
			
		const float Dot = FVector::DotProduct(Player->GetActorForwardVector(), DirArrow);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), Dot));

		if (Dot > 0.7f)
		{
			if (BBComp)
			{
				BBComp->SetValueAsBool("bIsSpotted", true);
			}
			
			if (WorldSystem)
			{
				WorldSystem->InitiateRandomEvent();
			}
			
			SetActorTickEnabled(false);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Called")));
		}
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::MoveToHidingSpot(const ERandomEvent SelRandEvent)
{
	if (HidingSpotLocations.IsEmpty()) return;
	if (!IsValid(Player)) return;
	
	if (SelRandEvent == ERandomEvent::HidingJumpscare)
	{
		//Enable Tick For Looking At Enemy
		SetActorTickEnabled(true);
		
		const FVector PlayerLoc = Player->GetActorLocation();
		float ClosestDist = MAX_FLT;
		FVector ClosestLoc;
		
		for (const ATargetPoint* TargetPoint : HidingSpotLocations)
		{
			const FVector HidingLoc = TargetPoint->GetActorLocation();
			const float ClosestSquared = FVector::DistSquared(PlayerLoc, HidingLoc);
			
			if (ClosestSquared < ClosestDist)
			{
				ClosestDist = ClosestSquared;
				ClosestLoc = HidingLoc;
			}
			SetActorLocation(ClosestLoc);
			
			FTimerHandle SpottedTimerHandle;
			GetWorldTimerManager().SetTimer(SpottedTimerHandle, [this]
			{
				BBComp->SetValueAsBool("bIsSneaked", true);
			},
			CaughtTimer,
			false
			);
		}
	}
}
