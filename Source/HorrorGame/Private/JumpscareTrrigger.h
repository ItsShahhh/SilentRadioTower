// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "JumpscareTrrigger.generated.h"

class ULevelSequence;

UCLASS()
class HORRORGAME_API AJumpscareTrrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AJumpscareTrrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent>RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent>SkeletalComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent>PreLoadBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh>SkeletalMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<ULevelSequence>LevSeq;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLookAtEnemy = false;
	
private:
	void PreLoad();
	void AssetLoad();
	void LookingAtJumpscare();
	
	UFUNCTION()
	void DestroyJumpscare();
	
	UPROPERTY()
	TObjectPtr<APlayerCameraManager>PlayerCam;
	UPROPERTY()
	TObjectPtr<ULevelSequence>CachedSeq;
	UPROPERTY()
	TArray<FSoftObjectPath>ListToLoad;
	
	bool bAllLoaded = false;
	
	UFUNCTION()
	void OnOverlapBegin(
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
