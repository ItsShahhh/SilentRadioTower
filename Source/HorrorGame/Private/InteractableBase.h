// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

class UCameraComponent;
class UWorldSystem;

UCLASS()
class AInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MainMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> MainCamera;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BlendSpeed = 1.0f;
	
private:
	UPROPERTY()
	TObjectPtr<AActor> InteractingActor;
	UPROPERTY()
	TObjectPtr<APawn> Pawn;
	
	FTimerHandle ObjectiveTimerHandle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override;
	virtual void ExitFromInteractable();
	
};
