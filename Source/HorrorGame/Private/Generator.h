// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UWorldSystem;

UCLASS()
class AGenerator : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> GeneratorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ValveMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ValveRotationAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ValveRotationSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BlendSpeed = 5.0f;
	
private:
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UPROPERTY()
	TObjectPtr<APawn> Pawn;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
	float ValveRotation;
	
	void RotateValve(const FInputActionValue& Value);
	void ExitFromGenerator();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
