// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "GeneratorMachine.generated.h"

enum class ERandomEvent : uint8;
class UCameraComponent;
class UInputAction;
/**
 * 
 */
UCLASS()
class AGeneratorMachine : public AInteractableBase
{
	GENERATED_BODY()
	
public:
	AGeneratorMachine();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> BigButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SmallButton1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SmallButton2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SmallButton3;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Input")
	TObjectPtr<UInputAction> ButtonInteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TObjectPtr<UPrimitiveComponent>, bool> ButtonsMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input")
	float LineTraceRange = 200.0f;
	
private:
	bool bCanPressed = false;
	
	void OnMouseClicked();
	void CheckButtons(const FHitResult& HitResult);
	
	UFUNCTION()
	void RandGeneratorPower(const ERandomEvent SelRanEvents);
	
public:
	virtual void Interactable_Implementation(AActor* InteractActor) override;

};
