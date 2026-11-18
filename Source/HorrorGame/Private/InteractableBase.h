// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"

class AHorrorGamePlayerController;
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Interactable_Implementation(AActor* InteractActor) override PURE_VIRTUAL(AInteractableActor::Interactable_Implementation, );
};
