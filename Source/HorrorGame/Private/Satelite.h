// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "GameFramework/Actor.h"
#include "Satelite.generated.h"

enum class ERandomEvent : uint8;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UWorldSystem;

UCLASS()
class ASatelite : public AInteractableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASatelite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ValveMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ValveRotationAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ValveRotationSpeed = 5.0f;
	
private:
	float ValveRotation;
	float SatelliteRotObj;
	
	void RotateValve(const FInputActionValue& Value);
	
	UFUNCTION()
	void RandSatelliteRot(const ERandomEvent SelRanEvents);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
