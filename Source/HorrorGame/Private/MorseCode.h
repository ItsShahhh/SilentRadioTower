// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "MorseCode.generated.h"

class UCameraComponent;
class UInputAction;
class UMorseCodeNumberWidget;
class USpotLightComponent;
class UWidgetComponent;

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
class AMorseCode : public AInteractableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMorseCode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpotLightComponent> MorseLight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ButtonInteractAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> LightButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Button1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Button2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Button3;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WidgetComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LongBeep = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ShortBeep = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LineTraceRange = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, FMorseSequence> MorseSignalMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32>CodeObjective;
	
private:
	UPROPERTY()
	TObjectPtr<UMorseCodeNumberWidget> MorseCodeNumberWidget;
	
	FTimerHandle MorseLightTimerHandle;
	FMorseSequence ChoosenMorseSignal;
	
	float NextSignal = 0;
	
	int32 CurrentMorseCode = 0;
	int32 CurNumBut1 = 0;
	int32 CurNumBut2 = 0;
	int32 CurNumBut3 = 0;
	
	bool bIsMachineActive;
	
	void ActivateMorseLight();
	void RandomizeMorseCode();
	void OnMouseClicked();
	void CheckCorrectCode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
