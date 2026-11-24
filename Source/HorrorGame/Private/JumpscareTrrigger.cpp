// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpscareTrrigger.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "QuestSubSystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJumpscareTrigger::AJumpscareTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	SkeletalComp->SetupAttachment(RootComp);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComp->SetupAttachment(RootComp);
	
	AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComp->SetupAttachment(RootComp);
	
	PreLoadBox = CreateDefaultSubobject<UBoxComponent>("PreLoadColl");
	PreLoadBox->SetupAttachment(RootComp);
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AJumpscareTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(SkeletalComp, TEXT("Failed To Find Skeletal Mesh Component In Jumpscare Trigger"));
	checkf(StaticMeshComp, TEXT("Failed To Find Static Mesh Component In Jumpscare Trigger"));
	checkf(AudioComp, TEXT("Failed To Find Audio Component In Jumpscare Trigger"));
	checkf(PreLoadBox, TEXT("Failed To Find PreloadBox Component In Jumpscare Trigger"));
	checkf(CollisionBox, TEXT("Failed To Find Collision Box Component In Jumpscare Trigger"));
	
	//Set Tick Disabled As Default For Optimization
	SetActorTickEnabled(false);
	
	switch (JumpscareType)
	{
	case EJumpscareType::VisionTrigger:
		
		//Cached Player Camera
		PlayerCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (!ensureAlwaysMsgf(PlayerCam, TEXT("Failed To Find PLayer Camera In Jumpscare Trigger"))) return;
		
		if (!ensureAlwaysMsgf(!SkeletalMesh.IsNull(), TEXT("Failed To Find Path Skeletal Mesh In Jumpscare Trigger"))) return;
		if (!ensureAlwaysMsgf(!LevSeq.IsNull(), TEXT("Failed To Find Path Level Sequence In Jumpscare Trigger"))) return;
		
		//If Look At Enemy True That Mean We Are Using The Tick
		//If Not The Tick Will Be Disabled
		SetActorTickEnabled(true);
		
		//Remove Component That We Don't Use For That Jumpscare
		StaticMeshComp->DestroyComponent();
		AudioComp->DestroyComponent();
		CollisionBox->DestroyComponent();
		break;
		
	case EJumpscareType::CollisionTrigger:
		
		if (!ensureAlwaysMsgf(!StatMesh.IsNull(), TEXT("Failed To Find Path Static Mesh In Jumpscare Trigger"))) return;
		if (!ensureAlwaysMsgf(!SoundBase.IsNull(), TEXT("Failed To Find Path Sound For Collision Trigger In Jumpscare Trigger"))) return;
		
		//Remove Component That We Don't Use For That Jumpscare
		SkeletalComp->DestroyComponent();
		break;
		
	case EJumpscareType::SoundTrigger:
		
		if (!ensureAlwaysMsgf(!SoundBase.IsNull(), TEXT("Failed To Find Path Sound For Sound Trigger In Jumpscare Trigger"))) return;
		
		//Remove Component That We Don't Use For That Jumpscare
		SkeletalComp->DestroyComponent();
		StaticMeshComp->DestroyComponent();
		break;
		
	default:
		break;
	}
	
	if (bQuestLoad)
	{
		if (UQuestSubSystem* QuestSubSystem = GetGameInstance()->GetSubsystem<UQuestSubSystem>())
		{
			QuestSubSystem->OnCheckJumpscare.AddDynamic(this, &AJumpscareTrigger::QuestPreLoad);
		}
		
		PreLoadBox->DestroyComponent();
	}
	
	if (PreLoadBox)
	{
		PreLoadBox->OnComponentBeginOverlap.AddDynamic(this, &AJumpscareTrigger::ColPreLoad);
	}
}

// Called every frame
void AJumpscareTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bAllLoaded)
	{
		LookingAtJumpscare();
	}
}

void AJumpscareTrigger::ColPreLoad(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Unbind From PreloadBox So It Doesn't Trigger Twice
	PreLoadBox->OnComponentBeginOverlap.RemoveDynamic(this, &AJumpscareTrigger::ColPreLoad);

	if (OtherActor->ActorHasTag("Player"))
	{
		Preload();
	}
}

void AJumpscareTrigger::QuestPreLoad(const FGameplayTag& QuestTag)
{
	if (!ensureMsgf(QuestTriggerTag.IsValid(), TEXT("Forgot To Set Quest Trigger Tag In Jumpscare Trigger"))) return;
	
	if (QuestTriggerTag.MatchesTag(QuestTag))
	{
		Preload();
	}
}

void AJumpscareTrigger::Preload()
{
	TArray<FSoftObjectPath> ListToLoad;
		
	switch (JumpscareType)
	{
	case EJumpscareType::VisionTrigger:
		
		ListToLoad.Add(SkeletalMesh.ToSoftObjectPath());
		ListToLoad.Add(LevSeq.ToSoftObjectPath());
		break;
		
	case EJumpscareType::CollisionTrigger:
		
		ListToLoad.Add(StatMesh.ToSoftObjectPath());
		ListToLoad.Add(SoundBase.ToSoftObjectPath());
		break;
		
	case EJumpscareType::SoundTrigger:
		
		ListToLoad.Add(SoundBase.ToSoftObjectPath());
		break;
		
	default:
		break;
	}
	
	PreLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(ListToLoad, FStreamableDelegate::CreateUObject(this, &AJumpscareTrigger::AssetLoad));
}

void AJumpscareTrigger::AssetLoad()
{
	switch (JumpscareType)
	{
	case EJumpscareType::VisionTrigger:
		{
			ULevelSequence* Seq = LevSeq.Get();
			if (!ensureAlwaysMsgf(Seq, TEXT("Failed To Get Level Sequence Asset Load In Jumpscare Trigger"))) return;
			CachedSeq = Seq;
	
			USkeletalMesh* Mesh = SkeletalMesh.Get();
			if (!ensureAlwaysMsgf(Mesh, TEXT("Failed To Get Skeletal Mesh Asset Load In Jumpscare Trigger"))) return;
			SkeletalComp->SetSkeletalMeshAsset(Mesh);
			
			bAllLoaded = true;
			break;
		}
	
	case EJumpscareType::CollisionTrigger:
		{
			USoundBase* Sound = SoundBase.Get();
			if (!ensureAlwaysMsgf(Sound, TEXT("Failed To Get Sound Asset Load In Jumpscare Trigger"))) return;
			AudioComp->SetSound(Sound);
			
			UStaticMesh* StaticMesh = StatMesh.Get();
			if (!ensureAlwaysMsgf(StaticMesh, TEXT("Failed To Get Static Mesh Asset Load In Jumpscare Trigger"))) return;
			StaticMeshComp->SetStaticMesh(StaticMesh);
			break;
		}
	
	case EJumpscareType::SoundTrigger:
		{
			USoundBase* Sound = SoundBase.Get();
			if (!ensureAlwaysMsgf(Sound, TEXT("Failed To Get Sound Asset Load In Jumpscare Trigger"))) return;
			AudioComp->SetSound(Sound);
			break;
		}
		
	default:
		break;
	}
	
	//This Is For Audio and Object Trigger
	//Only Bind This When Asset Is Load To Prevent Player From Overlap With Collision Box Before Asset Load 
	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AJumpscareTrigger::CollisionTrigger);
	}
}

void AJumpscareTrigger::CollisionTrigger(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Unbind Coliison Box From Collision Trigger
	CollisionBox->OnComponentBeginOverlap.RemoveDynamic(this, &AJumpscareTrigger::CollisionTrigger);
	
	if (OtherActor->ActorHasTag("Player"))
	{
		switch (JumpscareType)
		{
		case EJumpscareType::CollisionTrigger:
			
			ObjectJumpscare();
			break;
			
		case EJumpscareType::SoundTrigger:
			
			SoundJumpscare();
			break;
			
		default:
			break;
		}
	}
}

void AJumpscareTrigger::ObjectJumpscare()
{
	StaticMeshComp->SetSimulatePhysics(true);
	AudioComp->Play();
	
	//Static Mesh Is Stayed In The Level To Make Sure It Doesn't Take Too Much RAM We Remove The Not Essential Components
	PreLoadBox->DestroyComponent();
	CollisionBox->DestroyComponent();
	
	if (!ensureAlwaysMsgf(PreLoadHandle, TEXT("Failed To Get Load Asset In Jumpscare Trigger"))) return;
	PreLoadHandle->ReleaseHandle();
	PreLoadHandle.Reset();
}

void AJumpscareTrigger::SoundJumpscare()
{
	AudioComp->Play();
	AudioComp->OnAudioFinished.AddDynamic(this, &AJumpscareTrigger::DestroyJumpscare);
}

void AJumpscareTrigger::LookingAtJumpscare()
{
	//Only Calling This Function If bLookAtEnemy Is True
	const FVector CamFor = PlayerCam->GetActorForwardVector();
	const FVector CamLoc = PlayerCam->GetCameraLocation();
	
	const FVector SkelLoc = SkeletalComp->GetComponentLocation();
	const FVector SkelDir = (SkelLoc - CamLoc).GetSafeNormal();
	
	const float Dot = FVector::DotProduct(CamFor, SkelDir);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%f"), Dot));
	
	if (Dot >= 0.8f)
	{
		const FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* OutSequenceActor = nullptr;
		ULevelSequencePlayer* PlaySeq = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),CachedSeq, PlaybackSettings, OutSequenceActor);
		
		PlaySeq->Play();
		PlaySeq->OnFinished.AddDynamic(this, &AJumpscareTrigger::DestroyJumpscare);
	}
}

void AJumpscareTrigger::DestroyJumpscare()
{
	Destroy();
}
