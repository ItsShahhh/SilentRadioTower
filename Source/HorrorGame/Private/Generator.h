// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "Generator.generated.h"

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
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
private:
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
