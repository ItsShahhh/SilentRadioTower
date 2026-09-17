// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldEventBase.h"

#include "InteractableInterface.h"
#include "Components/BoxComponent.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "PlayerMode.h"
#include "WorldSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AWorldEventBase::ComponentBeginOverlap);
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (bActivateOverlap)
	{
		BoxComp->SetVisibility(true);
	}
	else
	{
		BoxComp->SetVisibility(false);
	}
	
	//Enable and Disable Tick Based On Rendered Trigger To Save Perfomance
	//Because In The Game There Going To be Alot Of World Event
	//Maybe Can Try When Report Event Then Need To Enable Actor Collision and Stuff From There Enabled Tick Based On Tag
	if (bRenderedTrigger)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

// Called every frame
void AWorldEventBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bRenderedTrigger)
	{
		if (SequenceActor && Character)
		{
			if (SequenceActor->WasRecentlyRendered(0.1f))
			{
				const FVector CharacterForward = Character->GetActorForwardVector();
				
				const FVector CharacterLocation = Character->GetActorLocation();
				const FVector SeqActorLocation = SequenceActor->GetActorLocation();
				const FVector DirectionToSequence = (SeqActorLocation - CharacterLocation).GetSafeNormal();
				
				const float Dot = FVector::DotProduct(CharacterForward, DirectionToSequence);
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), Dot));
				
				if (Dot > RenderTriggerRange)
				{
					if (!bHasPlay)
					{
						TriggerSequencer();
						
						//Disable Tick When Sequence Trigger
						SetActorTickEnabled(false);
					}
				}
			}
		}
	}
}

void AWorldEventBase::TriggerSequencer()
{
	//If Sequence Event is False That Meant This World Event Is Inly For Triggering Quest
	if (!SequenceEvent) return;
	
	//Check If Level Sequence Is Already Triggered
	if (bHasPlay) return;
	
	if (!IsValid(SequenceActor)) return;
	
	//Create Level Seuquence Player
	ALevelSequenceActor* OutActor;
	FMovieSceneSequencePlaybackSettings Settings;
	
	//Keep The Last Frame From The Level Sequence Instead Of Restore Back
	Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
	
	if (ULevelSequencePlayer* SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, OutActor))
	{
		//Play The Level Sequence
		SeqPlayer->Play();
		
		//Set Has Play Boolean To True So If Used Render Sequeunce It's Prevent It From Getting Called Many Times
		bHasPlay = true;
		
		SeqPlayer->OnNativeFinished.BindWeakLambda(this, [this]
		{
			//If True Than When Seuquence Finish The Player Should Rotate To Look At Actor
			if (bLookAtActor)
			{
				if (Character && Character->Implements<UPlayerMode>())
				{
					IPlayerMode::Execute_LookAtActor(Character, SequenceActor);
				}
			}
			
			if (WorldSystem)
			{
				if (SequenceActor->Implements<UInteractableInterface>())
				{
					//Getting Gameplay Tag For Dialogue From Sequence Actor That We Reference In The World
					if (const FGameplayTag DialogueTag = IInteractableInterface::Execute_GetGameplayTag(SequenceActor); DialogueTag.IsValid())
					{
						//If The Sequence Doesn't Have a Dialogue Just Leave Dialogue Tag None, So This Doesn't Get Called
						WorldSystem->PlayDialogueData(DialogueTag);
					}
				}
			}
		});
	}
}

void AWorldEventBase::TriggerQuest() const
{
	if (!QuestEvent) return;
	if (!WorldSystem) return;
	
	WorldSystem->ReportQuest();
}


void AWorldEventBase::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerSequencer();
	TriggerQuest();
}
