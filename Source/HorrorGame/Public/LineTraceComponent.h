// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LineTraceComponent.generated.h"

class UWorldSystem;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULineTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LineTraceRange = 100.0f;
	
private:
	FHitResult HitResult;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UPROPERTY()
	TObjectPtr<APawn> Pawn;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
	bool bHit;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CalledTraceLine() const;
	
};
