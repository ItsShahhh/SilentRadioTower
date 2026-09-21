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
	
	if (WorldSystem)
	{
		WorldSystem->OnCompletedQuest.AddDynamic(this, &AWorldEventBase::ToggleWorldEvent);
	}

#pragma region Importan Checks
	
	//Empty Bool
	bool bImportanChecks = false;
	
	if (!bEarlyTrigger && !bLaterTrigger)
	{
		bImportanChecks = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("For This Actor %s: Both Early and Late Trigger Is Not Set"), *GetActorNameOrLabel()));
	}
	
	if (!bRenderTriggered && !bActivateOverlap)
	{
		bImportanChecks = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("For This Actor %s: Both Render and Overlap Trigger Is Not Set"), *GetActorNameOrLabel()));
	}
	
	if (!bSequenceEvent && !bQuestEvent)
	{
		bImportanChecks = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("For This Actor %s: Both Sequence and Quest Event Is Not Set"), *GetActorNameOrLabel()));
	}
	
	if (!SequenceActor)
	{
		bImportanChecks = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("For This Actor %s: Sequence Actor Is Not Set"), *GetActorNameOrLabel()));
	}
	
	if (bImportanChecks)
	{
		SetActorTickEnabled(false);
		BoxComp->SetVisibility(false);
	}
#pragma endregion Importan Boolean Checks
	
	//Enable and Disable Tick Based On Rendered Trigger To Save Perfomance
	//If Using Later Rendered Trigger The Tick and Collision Box Only Avalaible After Finish Quest
	if (bLaterTrigger)
	{
		SetActorTickEnabled(false);
		
		if (BoxComp && SequenceActor)
		{
			BoxComp->SetVisibility(false);
			SequenceActor->SetActorHiddenInGame(true);
			SequenceActor->SetActorEnableCollision(false);
		}
	}
	else if (bEarlyTrigger)
	{
		SetActorTickEnabled(bRenderTriggered);
		
		if (BoxComp && SequenceActor)
		{
			BoxComp->SetVisibility(bActivateOverlap);
			SequenceActor->SetActorHiddenInGame(false);
			SequenceActor->SetActorEnableCollision(true);
		}
	}
}

// Called every frame
void AWorldEventBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

void AWorldEventBase::TriggerSequencer()
{
	//If Sequence Event is False That Meant This World Event Is Inly For Triggering Quest
	if (!bSequenceEvent) return;
	
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
	if (!bQuestEvent) return;
	if (!WorldSystem) return;
	
	WorldSystem->CompletedQuest();
}

void AWorldEventBase::ToggleWorldEvent(const FGameplayTag ReceiveQuestTag)
{
	if (!ReceiveQuestTag.IsValid()) return;
	if (!QuestTagForEvent.IsValid()) return;
	
	if (ReceiveQuestTag.MatchesTag(QuestTagForEvent))
	{
		if (bRenderTriggered)
		{
			SetActorTickEnabled(bRenderTriggered);
		}
		
		if (bActivateOverlap)
		{
			BoxComp->SetVisibility(bActivateOverlap);
		}
		
		if (SequenceActor)
		{
			SequenceActor->SetActorHiddenInGame(false);
			SequenceActor->SetActorEnableCollision(true);
		}
	}
}


void AWorldEventBase::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerSequencer();
	TriggerQuest();
}
