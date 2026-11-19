// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"

class UQuestSubSystem;

UENUM()
enum class EQuestType : uint8
{
	RequiredQuest UMETA(DisplayName = "Required Quest"),
	CompletedQuest UMETA(DisplayName = "Completed Quest"),
	None UMETA(DisplayName = "None")
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HORRORGAME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestComponent();

protected:
	UPROPERTY(EditAnywhere,  meta = (EditCondition = "QuestType == EQuestType::RequiredQuest", EditConditionHides))
	FGameplayTag RequiredQuestTag;
	
	UPROPERTY(EditAnywhere,  meta = (EditCondition = "QuestType == EQuestType::CompletedQuest", EditConditionHides))
	FGameplayTag CompletedQuestTag;
	
private:
	UPROPERTY()
	TObjectPtr<UQuestSubSystem>QuestSubSystem;

public:
	UPROPERTY(EditDefaultsOnly)
	EQuestType QuestType;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
	
	void CompletedQuest() const;
	bool RequiredQuest() const;
};
