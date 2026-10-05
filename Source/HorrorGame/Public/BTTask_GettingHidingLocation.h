// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_GettingHidingLocation.generated.h"

class ATargetPoint;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_GettingHidingLocation : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HidingLocationKey;
	
private:
	TArray<TObjectPtr<ATargetPoint>> HidingTargetPoints;
	
	UPROPERTY()
	TObjectPtr<APawn> Enemy;
};
