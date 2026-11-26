// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleInteractables.h"
#include "SignalWave.generated.h"

class UArrowComponent;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;

UCLASS()
class ASignalWave : public APuzzleInteractables
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASignalWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	TObjectPtr<UStaticMeshComponent> KnobMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Inputs")
	TObjectPtr<UInputAction> MouseRotateAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KnobRotationSpeed = 5.0f;
	
private:
	void RandomRotationObjective();
	void RotateKnob(const FInputActionValue& Value);
	
	float MouseKnobRotation = 0.0f;
	float KnobRotationObjective = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
