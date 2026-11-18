// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "PlayerMode.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HorrorGameCharacter.generated.h"

class AHorrorGamePlayerController;
class UGameplayMenuWidget;
class UAudioMenuWidget;
class UQuestWidget;
class UDialogueWidget;
class ULineTraceComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */

UCLASS(abstract)
class AHorrorGameCharacter : public ACharacter, public IPlayerMode, public IInteractableInterface
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MouseLookAction;
	
	/** Interact Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, Category ="Components")
	ULineTraceComponent* LineTraceComponent;

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget")
	TSubclassOf<UQuestWidget> QuestWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Variables")
	float CameraSpeed = 5.0f;

private:
	FRotator CurrentCameraRotation;
	FRotator TargetCameraRotation;
	
	FTimerHandle DialogueTimerHandle;
	
	bool bDialogueMode = false;
	
	UPROPERTY()
	TObjectPtr<UQuestWidget> QuestWidget;
	UPROPERTY()
	TObjectPtr<AHorrorGamePlayerController> PC;
	
	void Interact();
	
public:
	AHorrorGameCharacter();

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void LookAtActor_Implementation(const AActor* LookAtActor) override;
};

