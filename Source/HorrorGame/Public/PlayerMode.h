// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerMode.generated.h"

struct FDialogueLine;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerMode : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORRORGAME_API IPlayerMode
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TalkToNPC(const FName& BoneDirection, const ACharacter* CurrentCharacter);
	
};
