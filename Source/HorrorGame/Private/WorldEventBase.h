// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	TObjectPtr<AActor> SequenceActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	TObjectPtr<ULevelSequence> LevelSequence;
	
	//Enable This To Move Player Camera To Actor After Finished Sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	bool bLookAtActor = false;
	
	//Enable This If Want To Make Sequence Play When See An Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	bool bRenderedTrigger = false;
	
	//Enable This If Want To Make Sequence Play When See An Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	bool bActivateOverlap = false;
	
	//Enable This If Want To Make Sequence Play When See An Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	float RenderTriggerRange = 0.8f;

private:
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void TriggerSequencer();
	
	FTimerHandle SequenceTimerHandle;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UPROPERTY()
	TObjectPtr<ACharacter> Character;
	
	bool bHasPlay = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
