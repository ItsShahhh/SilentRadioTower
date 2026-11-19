// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "KeyBase.generated.h"

class UQuestComponent;

UCLASS()
class HORRORGAME_API AKeyBase : public AInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKeyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UQuestComponent>QuestComponent;
	
private:
	void PickupKey();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override;
};
