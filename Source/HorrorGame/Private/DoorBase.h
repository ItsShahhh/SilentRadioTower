// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "DoorBase.generated.h"

class UBoxComponent;
class UQuestComponent;

UCLASS()
class HORRORGAME_API ADoorBase : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UQuestComponent> QuestComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DoorRotSpeed = 5.0f;
	
private:
	FRotator TargetDoorRot;
	FRotator CurrentDoorRot;
	
	bool bDoorOpen = false;
	
	void OpenDoor(const AActor* InteractActor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override;
};
