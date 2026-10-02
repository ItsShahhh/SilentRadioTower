// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightBase.generated.h"

enum class ERandomEvent : uint8;
class USpotLightComponent;
class UWorldSystem;

UCLASS()
class ALightBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpotLightComponent> LightComp;
	
private:
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UFUNCTION()
	void ToggleLight(const bool ToggleLight) const;
	
	UFUNCTION()
	void RandEventPower(const ERandomEvent SelRandEvent) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
