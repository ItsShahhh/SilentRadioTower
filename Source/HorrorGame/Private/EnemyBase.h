// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class UBlackboardComponent;
class ATargetPoint;
class AAIController;
enum class ERandomEvent : uint8;
class UWorldSystem;

UCLASS()
class HORRORGAME_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BehindPlayerRange = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ATargetPoint>> HidingSpotLocations;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> Player;
	
	UPROPERTY()
	TObjectPtr<AAIController> AIController;
	
	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BBComp;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UFUNCTION()
	void MoveToHidingSpot(const ERandomEvent SelRandEvent);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
