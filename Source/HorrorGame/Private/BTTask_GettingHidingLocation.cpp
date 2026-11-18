// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GettingHidingLocation.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_GettingHidingLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!Enemy)
	{
		const AAIController* AIController = OwnerComp.GetAIOwner();
		if (!AIController) return EBTNodeResult::Failed;
	
		APawn* Pawn = AIController->GetPawn();
		if (!Pawn) return EBTNodeResult::Failed;
		
		Enemy = Pawn;
	}
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	
	if (HidingTargetPoints.IsEmpty())
	{
		for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
		{
			if (ATargetPoint* TargetPoint = *It)
			{
				HidingTargetPoints.Add(TargetPoint);
			}
		}
	}
	
	if (!Enemy) return EBTNodeResult::Failed;
	
	const FVector EnemyLoc = Enemy->GetActorLocation();
	float ClosestDist = MAX_FLT;
	FVector ClosestLoc = FVector::ZeroVector;
		
	for (const ATargetPoint* HidingLocation : HidingTargetPoints)
	{
		const FVector HidingLoc = HidingLocation->GetActorLocation();
		const float ClosestSquared = FVector::DistSquared(EnemyLoc, HidingLoc);
			
		if (ClosestSquared < ClosestDist)
		{
			ClosestDist = ClosestSquared;
			ClosestLoc = HidingLoc;
		}
	}
	BBComp->SetValueAsVector(HidingLocationKey.SelectedKeyName, ClosestLoc);
	return EBTNodeResult::Succeeded;
}
