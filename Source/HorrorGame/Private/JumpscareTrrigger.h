// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "JumpscareTrrigger.generated.h"

struct FStreamableHandle;
class ULevelSequence;

UENUM()
enum class EJumpscareType : uint8
{
	VisionTrigger UMETA(DisplayName = "VisionTrigger"),
	CollisionTrigger UMETA(DisplayName = "CollisionTrigger"),
	SoundTrigger UMETA(DisplayName = "SoundTrigger"),
};

UCLASS()
class HORRORGAME_API AJumpscareTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJumpscareTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent>RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent>SkeletalComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent>StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAudioComponent>AudioComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent>PreLoadBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent>CollisionBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Soft Pointer", meta = (EditCondition = "JumpscareType == EJumpscareType::VisionTrigger", EditConditionHides))
	TSoftObjectPtr<USkeletalMesh>SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Soft Pointer", meta = (EditCondition = "JumpscareType == EJumpscareType::VisionTrigger", EditConditionHides))
	TSoftObjectPtr<ULevelSequence>LevSeq;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Soft Pointer", meta = (EditCondition = "JumpscareType == EJumpscareType::CollisionTrigger", EditConditionHides))
	TSoftObjectPtr<UStaticMesh>StatMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Soft Pointer", meta = (EditCondition = "JumpscareType == EJumpscareType::CollisionTrigger && JumpscareType == EJumpscareType::SoundTrigger", EditConditionHides))
	TSoftObjectPtr<USoundBase>SoundBase;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "JumpscareType", meta = (EditCondition = "bQuestLoad", EditConditionHides))
	FGameplayTag QuestTriggerTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "JumpscareType")
	EJumpscareType JumpscareType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "JumpscareType")
	bool bQuestLoad = false;
	
private:
	void AssetLoad();
	void Preload();
	void ObjectJumpscare();
	void SoundJumpscare();
	void LookingAtJumpscare();
	
	UFUNCTION()
	void QuestPreLoad(const FGameplayTag& QuestTag);
	
	UFUNCTION()
	void DestroyJumpscare();
	
	UPROPERTY()
	TObjectPtr<APlayerCameraManager>PlayerCam;
	UPROPERTY()
	TObjectPtr<ULevelSequence>CachedSeq;
	
	TSharedPtr<FStreamableHandle>PreLoadHandle;
	
	bool bAllLoaded = false;
	
	UFUNCTION()
	void ColPreLoad(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UFUNCTION()
	void CollisionTrigger(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
