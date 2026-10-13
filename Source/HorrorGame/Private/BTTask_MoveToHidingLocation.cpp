// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToHidingLocation.h"
#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_MoveToHidingLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!Enemy && !Player)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		if (!AIController) return EBTNodeResult::Failed;
	
		Enemy = AIController->GetPawn();
		if (!Enemy) return EBTNodeResult::Failed;
		
		Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (!Player) return EBTNodeResult::Failed;
	}
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	
	//Check If Target Point Already Set From Before
	//If Not Empty That Mean Its Already Set So Don't Have To Run Loop Again
	if (HidingLocations.IsEmpty())
	{
		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			if (ATargetPoint* TargetPoint = *It)
			{
				HidingLocations.Add(TargetPoint);
			}
		}
	}
	
	//Check If Hiding Location Is Still Empty It Will Abort
	if (HidingLocations.IsEmpty()) return EBTNodeResult::Failed;
	
	const FVector PlayerLoc = Player->GetActorLocation();
	float ClosestDist = MAX_FLT;
	FVector ClosestLoc;
		
	for (const ATargetPoint* TargetPoint : HidingLocations)
	{
		const FVector HidingLoc = TargetPoint->GetActorLocation();
		const float ClosestSquared = FVector::DistSquared(PlayerLoc, HidingLoc);
			
		if (ClosestSquared < ClosestDist)
		{
			ClosestDist = ClosestSquared;
			ClosestLoc = HidingLoc;
		}
	}
		
	const FRotator TurnToPlayer = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), PlayerLoc);
	
	Enemy->SetActorLocation(ClosestLoc);
	Enemy->SetActorRotation(TurnToPlayer);
	
	return EBTNodeResult::Succeeded;
}
