// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_MoveToHidingLocation.generated.h"

class ATargetPoint;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_MoveToHidingLocation : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<ACharacter>Player;
	
	UPROPERTY()
	TObjectPtr<APawn>Enemy;
	
	TArray<TObjectPtr<ATargetPoint>>HidingLocations;
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
