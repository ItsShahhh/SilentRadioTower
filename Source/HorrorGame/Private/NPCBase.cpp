// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "PlayerMode.h"
#include "WorldSystem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
}

// Called every frame
void ANPCBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsInteract)
	{
		CurrentNPCRotation = UKismetMathLibrary::RInterpTo(CurrentNPCRotation, TargetNPCRotation, DeltaTime, NPCRotateSpeed);
		SetActorRotation(FRotator(0.0f, CurrentNPCRotation.Yaw, 0.0f));
	}
	
	if (CurrentNPCRotation.Equals(TargetNPCRotation, 0.1))
	{
		bIsInteract = false;
		SetActorTickEnabled(false);
	}
}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPCBase::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;
	
	//Enable Event Tick For Enemy
	SetActorTickEnabled(true);
	
	CurrentNPCRotation = GetActorRotation();
	TargetNPCRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InteractActor->GetActorLocation());
	bIsInteract = true;
	
	if (WorldSystem)
	{
		//If One Of These Doesn't Have A Tag It Will Skip It
		//Reminder: If The NPC Only Have Dialogue Or Quest Leave That Specific Tag Clear
		
		if (DialogueTag.IsValid())
		{
			WorldSystem->PlayDialogueData(DialogueTag);
		}
		
		if (QuestTag.IsValid())
		{
			WorldSystem->StartQuest(QuestTag);
		}
	}
	
	if (InteractActor->Implements<UPlayerMode>())
	{
		//Give The NPC Head Player Bone Because That What Default Bone To Look At When Talking To NPC
		const FName HeadBone = GetMesh()->GetSocketBoneName("head");
		
		IPlayerMode::Execute_TalkToNPC(InteractActor, HeadBone, this);
	}
}


