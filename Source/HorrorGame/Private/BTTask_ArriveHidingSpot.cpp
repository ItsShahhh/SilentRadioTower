// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ArriveHidingSpot.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ArriveHidingSpot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	
	BBComp->SetValueAsBool(SpottedBoolKey.SelectedKeyName, false);
	BBComp->SetValueAsBool(SneakedBoolKey.SelectedKeyName, false);
	return EBTNodeResult::Succeeded;
}
