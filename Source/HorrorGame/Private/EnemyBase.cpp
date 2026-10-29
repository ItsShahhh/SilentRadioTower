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
	
	checkf(Player, TEXT("Failed To Get Player Character In EnemyBase"));
	checkf(WorldSystem, TEXT("Failed To Get World System In EnemyBase"));
	
	AIController = Cast<AAIController>(GetController());
	
	checkf(AIController,TEXT("Failed To Get AI Controller In EnemyBase"));
	
	BBComp = AIController->GetBlackboardComponent(); 
	
	checkf(BBComp,TEXT("Failed To Get BlackBoard In EnemyBase"));
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const FVector EnemLoc = GetActorLocation();
	const FVector PlayerLoc = Player->GetActorLocation();
			
	const FVector DirArrow = ( EnemLoc - PlayerLoc ).GetSafeNormal(); 
			
	const float Dot = FVector::DotProduct(Player->GetActorForwardVector(), DirArrow);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), Dot));

	if (Dot > 0.7f)
	{
		BBComp->SetValueAsBool("bIsSpotted", true);
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}