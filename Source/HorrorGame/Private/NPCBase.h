// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractableInterface.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

class UWorldSystem;

UCLASS()
class ANPCBase : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> LookAtComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> DialogueTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag NPCTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NPCRotateSpeed = 5.0f;
	
private:
	FRotator CurrentNPCRotation;
	FRotator TargetNPCRotation;
	bool bIsInteract = false;
	
	FTimerHandle TimerHandle;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Interactable_Implementation(AActor* InteractActor) override;
};
