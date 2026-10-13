// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_StartSneakTimer.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_StartSneakTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	
	GetWorld()->GetTimerManager().SetTimer(SneakTimerHandle, [this]
	{
		BBComp->SetValueAsBool(SneakBoolKey.SelectedKeyName, true);
		return EBTNodeResult::Succeeded;
	},
	StartSneakTimer,
	false
	);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_StartSneakTimer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!BBComp) return EBTNodeResult::Failed;
	
	GetWorld()->GetTimerManager().ClearTimer(SneakTimerHandle);
	return EBTNodeResult::Aborted;
}
