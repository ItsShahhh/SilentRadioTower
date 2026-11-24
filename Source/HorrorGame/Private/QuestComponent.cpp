// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "QuestSubSystem.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	QuestSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubSystem>();
	checkf(QuestSubSystem, TEXT("Failed To Get Quest Subsytem In Quest Component"));
}

// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::CompletedQuest() const
{
	if (!ensureAlwaysMsgf(CompletedQuestTag.IsValid(), TEXT("Forgot To Set Completed Quest Tag In Quest Component"))) return;
	
	QuestSubSystem->CompletedQuest(CompletedQuestTag);
	
	//Destroy Interact Quest Item Only After Calling Completed Quest To Make Sure It Still Get The Tag
	GetOwner()->Destroy();
}

bool UQuestComponent::RequiredQuest() const
{
	if (!ensureAlwaysMsgf(RequiredQuestTag.IsValid(), TEXT("Forgot To Set Request Quest Tag In Quest Component"))) return false;
	
	if (QuestSubSystem->FinishedQuestTag.HasTag(RequiredQuestTag))
	{
		return true;
	}
	
	return false;
}