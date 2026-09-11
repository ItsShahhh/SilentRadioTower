// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEventBase.h"

#include "InteractableInterface.h"
#include "Components/BoxComponent.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "PlayerMode.h"
#include "WorldSystem.h"

// Sets default values
AWorldEventBase::AWorldEventBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(RootComp);
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AWorldEventBase::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWorldEventBase::TriggerSequencer);
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
}

// Called every frame
void AWorldEventBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldEventBase::TriggerSequencer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check If Level Sequence Is Already Triggered
	if (bHasPlay) return;
	
	//Only Set PC Variable Once
	if (!PC)
	{
		if (const APawn* Pawn = Cast<APawn>(OtherActor))
		{
			PC = Pawn->GetLocalViewingPlayerController();
		}
	}
	
	if (!IsValid(PC)) return;
	if (!IsValid(GenericActor)) return;
	
	//When Trigger Disable Both Mouse and Keyboard
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);
	
	//Create Level Seuquence Player
	ALevelSequenceActor* OutActor;
	FMovieSceneSequencePlaybackSettings Settings;
	
	//Keep The Last Frame From The Level Sequence Instead Of Restore Back
	Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
	
	if (ULevelSequencePlayer* SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, OutActor))
	{
		//Play The Level Sequence
		SeqPlayer->Play();
		
		SeqPlayer->OnNativeFinished.BindWeakLambda(this, [this, OtherActor]
		{
			if (OtherActor->Implements<UPlayerMode>())
			{
				IPlayerMode::Execute_LookAtActor(OtherActor, GenericActor);
			}
			
			if (WorldSystem)
			{
				if (GenericActor->Implements<UInteractableInterface>())
				{
					if (const FGameplayTag DialogueTag = IInteractableInterface::Execute_GetGameplayTag(GenericActor); DialogueTag.IsValid())
					{
						//If The Sequence Doesn't Have a Dialogue Just Leave It None, So This Doesn't Get Called
						WorldSystem->PlayDialogueData(DialogueTag);
					}
				}
			}
			
			PC->SetIgnoreLookInput(false);
			PC->SetIgnoreMoveInput(false);
			bHasPlay = true;
		});
	}
}

