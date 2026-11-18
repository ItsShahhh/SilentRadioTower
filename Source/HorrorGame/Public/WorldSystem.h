// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestData.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSystem.generated.h"

enum class ERandomEvent : uint8;
class UGameSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateLight, const bool, ToggleLight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateRandomEvent, const ERandomEvent, RandomEvent);
/**
 * 
 */

UCLASS()
class HORRORGAME_API UWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	bool bAlreadyInitiated = false;
	
	UPROPERTY()
	const UGameSettings* GameSettings;
	
public:
	FOnUpdateLight OnUpdateLight;
	FOnUpdateRandomEvent OnUpdateRandomEvent;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void InitiateRandomEvent();
};
