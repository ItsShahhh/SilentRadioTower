// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "AIController.h"
#include "RandomEvent.h"
#include "WorldSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


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
		WorldSystem->OnUpdateRandomEvent.AddDynamic(this, &AEnemyBase::MoveBehindPlayer);
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

void AEnemyBase::MoveBehindPlayer(const ERandomEvent SelRandEvent)
{
	if (SelRandEvent == ERandomEvent::BehindJumpscare) 
	{
		const FVector PlayerLoc = Player->GetActorLocation();
		const FVector PlayerFor = -Player->GetActorForwardVector();
		const FVector FinalPlayerLoc = PlayerLoc + (PlayerFor * BehindPlayerRange);
		const FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLoc);
		
		SetActorLocation(FinalPlayerLoc);
		SetActorRotation(PlayerRot);
		
		SetActorTickEnabled(true);
		
		if (BBComp)
		{
			BBComp->SetValueAsBool("bIsSpotted", false);
		}
		
		//Disable Set Actor After 10 Seconds
		//This Is To Make Sure Even When Player Got Jumpscare The Tick Is Off
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this]
		{
			SetActorTickEnabled(false);
		},
		10.0f,
		false
		);
	}
}