// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "AIController.h"
#include "WorldSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
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
	
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	AIController = Cast<AAIController>(GetController());
	
	if (IsValid(AIController))
	{
		BBComp = AIController->GetBlackboardComponent(); 
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
		}
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}