// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "WorldEventBase.generated.h"

class UWorldSystem;
class ULevelSequence;
class UBoxComponent;

UCLASS()
class HORRORGAME_API AWorldEventBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldEventBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> GenericActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<ULevelSequence> LevelSequence;
	
private:
	UFUNCTION()
	void TriggerSequencer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle SequenceTimerHandle;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	bool bHasPlay = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
