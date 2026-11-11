// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpscareTrrigger.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJumpscareTrrigger::AJumpscareTrrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>("RootComponent");
	SetRootComponent(RootComp);
	
	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	SkeletalComp->SetupAttachment(RootComp);
	
	PreLoadBox = CreateDefaultSubobject<UBoxComponent>("PreLoadColl");
	PreLoadBox->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AJumpscareTrrigger::BeginPlay()
{
	Super::BeginPlay();
	
	//If Look At Enemy True That Mean We Are Using The Tick
	//If Not The Tick Will Be Disabled
	SetActorTickEnabled(bLookAtEnemy);
	
	PreLoadBox->OnComponentBeginOverlap.AddDynamic(this, &AJumpscareTrrigger::OnOverlapBegin);
	
	checkf(!SkeletalMesh.IsNull(), TEXT("Failed To Find Path Skeletal Mesh In Jumpscate Trigger"));
	checkf(!LevSeq.IsNull(), TEXT("Failed To Find Path Level Sequence In Jumpscate Trigger"));
}

// Called every frame
void AJumpscareTrrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bAllLoaded)
	{
		LookingAtJumpscare();
	}
}

void AJumpscareTrrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		//Cached Player Camera
		PlayerCam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		
		PreLoad();
	}
}

void AJumpscareTrrigger::PreLoad()
{
	ListToLoad.Add(SkeletalMesh.ToSoftObjectPath());
	ListToLoad.Add(LevSeq.ToSoftObjectPath());
	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(ListToLoad, FStreamableDelegate::CreateUObject(this, &AJumpscareTrrigger::AssetLoad));
}

void AJumpscareTrrigger::AssetLoad()
{
	ULevelSequence* Seq = LevSeq.Get();
	if (!Seq) return;
	CachedSeq = Seq;
	
	USkeletalMesh* Mesh = SkeletalMesh.Get();
	if (!Mesh) return;
	SkeletalComp->SetSkeletalMeshAsset(Mesh);
	
	bAllLoaded = true;
}

void AJumpscareTrrigger::LookingAtJumpscare()
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
		
		ULevelSequencePlayer* PlaySeq = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(), CachedSeq, PlaybackSettings, OutSequenceActor);
		
		if (PlaySeq)
		{
			PlaySeq->Play();
			PlaySeq->OnFinished.AddDynamic(this, &AJumpscareTrrigger::DestroyJumpscare);
		}
	}
}

void AJumpscareTrrigger::DestroyJumpscare()
{
	Destroy();
}
