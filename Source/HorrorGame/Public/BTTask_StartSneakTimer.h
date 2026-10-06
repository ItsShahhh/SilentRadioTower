// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_StartSneakTimer.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_StartSneakTimer : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	UBTTask_StartSneakTimer();
	
protected:
	UPROPERTY(EditAnywhere)
	float StartSneakTimer = 0.0f;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SneakBoolKey;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SneakTimerBoolKey;
	
private:
	FTimerHandle SneakTimerHandle;
	
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BBComp;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
