// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartSneakTimer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_StartSneakTimer::UBTTask_StartSneakTimer()
{
}

EBTNodeResult::Type UBTTask_StartSneakTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return EBTNodeResult::Failed;
	
	GetWorld()->GetTimerManager().SetTimer(SneakTimerHandle, [this]
	{
		BBComp->SetValueAsBool(SneakBoolKey.SelectedKeyName, true);
		BBComp->SetValueAsBool(SneakTimerBoolKey.SelectedKeyName, false);
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
	
	BBComp->SetValueAsBool(SneakTimerBoolKey.SelectedKeyName, false);
	GetWorld()->GetTimerManager().ClearTimer(SneakTimerHandle);
	return EBTNodeResult::Aborted;
}
