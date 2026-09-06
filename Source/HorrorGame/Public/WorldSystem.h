// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSystem.generated.h"

struct FDialogueLine;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateLight);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnableDialogue, const FDialogueLine&, DialogueData);
/**
 * 
 */
UCLASS()
class HORRORGAME_API UWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	FOnUpdateLight OnUpdateLight;
	FOnEnableDialogue OnProcessDialogue;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void PlayDialogueData(const UDataTable* DialogueTable, const FGameplayTag DialogueTag) const;
	
};
