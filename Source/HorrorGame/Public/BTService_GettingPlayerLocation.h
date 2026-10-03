// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_GettingPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTService_GettingPlayerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector PlayerLocationKey;
	
private:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;
};
