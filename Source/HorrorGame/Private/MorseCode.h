// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "MorseCode.generated.h"

class USpotLightComponent;

UENUM(BlueprintType)
enum class EMorseSignal : uint8
{
	Dot,
	Dash
};

USTRUCT(BlueprintType)
struct FMorseSequence
{
	GENERATED_BODY()
	
	// This array will now be fully visible and editable in the Blueprint Editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EMorseSignal> Signals;
};

UCLASS()
class AMorseCode : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMorseCode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MachineMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpotLightComponent> MorseLight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LongBeep = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShortBeep = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, FMorseSequence> MorseSignalMap;
	
private:
	FTimerHandle MorseLightTimerHandle;
	
	FMorseSequence ChoosenMorseSignal;
	
	float NextSignal = 0;
	
	bool bIsMachineActive;
	
	void ActivateMorseLight();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
