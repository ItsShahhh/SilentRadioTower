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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> BoxComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	TObjectPtr<AActor> SequenceActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Sequence")
	TObjectPtr<ULevelSequence> LevelSequence;
	
	//Enable This To Move Player Camera To Actor After Finished Sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type Of Sequence")
	bool bLookAtActor = false;
	
	//Enable This If Want To Trigger Sequence Without Need To Complete Quest First
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type Of Sequence")
	bool bEarlyTrigger = false;
	
	//Enable This If Want To Trigger Sequence Only After Completing Quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type Of Sequence")
	bool bLaterTrigger = false;
	
	//Enable This If Want To Trigger Sequence Using Occlusion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type Of Sequence")
	bool bRenderTriggered = false;
	
	//Enable This If Want To Trigger Sequence When Overlapping With Box Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type Of Sequence")
	bool bActivateOverlap = false;
	
	//Enable This If Want World Event To Be Able To Report Completed Quest
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Type Of Event")
	bool bQuestEvent;
	
	//Enable This If Want World Event Base To Play Sequence
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Type Of Event")
	bool bSequenceEvent;
	
	//Range For How Close It Will Trigger Render For Sequence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Type Of Sequence")
	float RenderTriggerRange = 0.8f;
	
	//Set This To Quest Event Tag To Enable World Event After Completing That Certain Quest
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Tag")
	FGameplayTag QuestTagForEvent;

private:
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ToggleWorldEvent(const FGameplayTag ReceiveQuestTag);
	void TriggerSequencer();
	void TriggerQuest() const;
	
	FTimerHandle SequenceTimerHandle;
	bool bHasPlay = false;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
