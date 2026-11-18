// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "PuzzleInteractables.generated.h"

/**
 * 
 */
UCLASS()
class APuzzleInteractables : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	APuzzleInteractables();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> MainCamera;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	UPROPERTY()
	TObjectPtr<AHorrorGamePlayerController> PC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BlendSpeed = 1.0f;
	
private:
	UPROPERTY()
	TObjectPtr<AActor> CachedActor;
	
	FTimerHandle ObjectiveTimerHandle;
	
public:
	virtual void Interactable_Implementation(AActor* InteractActor) override;
	virtual void ExitFromInteractable();
};
