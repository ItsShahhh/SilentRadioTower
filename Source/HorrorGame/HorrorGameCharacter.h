// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMode.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HorrorGameCharacter.generated.h"

class UGameplayMenuWidget;
class UAudioMenuWidget;
class UControlMenuWidget;
class UWorldSystem;
class UOptionMenuWidget;
class UPauseMenuWidget;
class UCrosshairWidget;
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
class AHorrorGameCharacter : public ACharacter, public IPlayerMode
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
	
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* TogglePauseMenuAction;
	
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
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget")
	TSubclassOf<UQuestWidget> QuestWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Widget")
	TSubclassOf<UOptionMenuWidget> OptionMenuWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Variables")
	float CameraSpeed = 5.0f;

private:
	FRotator CurrentCameraRotation;
	FRotator TargetCameraRotation;
	
	FInputModeGameOnly GameMode;
	FInputModeUIOnly UIMode;
	FTimerHandle DialogueTimerHandle;
	
	bool bDialogueMode = false;
	bool bIsPauseMenuToggle = false;
	bool bIsOptionMenuToggle = false;
	
	float BasedSensitivity = 1.0f;
	
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	UPROPERTY()
	TObjectPtr<UOptionMenuWidget> OptionMenuWidget;
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidget;
	UPROPERTY()
	TObjectPtr<UQuestWidget> QuestWidget;
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CrosshairWidget;
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget;
	
	UFUNCTION()
	void ChangeSensitivity(const float NewSensValue);
	UFUNCTION()
	void GameModeState() const;
	UFUNCTION()
	void UIModeState() const;
	void Interact();
	void TogglePauseMenu();
	
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

