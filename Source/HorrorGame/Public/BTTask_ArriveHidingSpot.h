// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_ArriveHidingSpot.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_ArriveHidingSpot : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SpottedBoolKey;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SneakedBoolKey;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
