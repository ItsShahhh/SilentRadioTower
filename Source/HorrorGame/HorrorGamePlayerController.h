// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorrorGamePlayerController.generated.h"

class AHorrorCharacter;
class UCrosshairWidget;
enum class ESettingMenus : uint8;
struct FStreamableHandle;
class UOptionMenuWidget;
class UPauseMenuWidget;
class UInputAction;
class UInputMappingContext;
class UUserWidget;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */

UCLASS(abstract, config="Game")
class HORRORGAME_API AHorrorGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	AHorrorGamePlayerController();

protected:
	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="User Widget")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category="User Widget")
	TSubclassOf<UUserWidget>OptionMenuClass;
	
	UPROPERTY(EditDefaultsOnly, Category="User Widget")
	TSubclassOf<UUserWidget>CrosshairClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Input Actions")
	TObjectPtr<UInputAction>TogglePause;
	
	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

public:
	void ToggleCrosshair(const bool bToggleCrosshair) const;
	void InteractingMode(AActor* InteractingActor,const float Blendspeed);
	void UnInteractingMode(AActor* InteractingActor,const float Blendspeed);
	
	float FinalSensValue = 1.0;
	
private:
	UFUNCTION()
	void TogglePauseMenu();
	UFUNCTION()
	void ToggleWidget(const ESettingMenus CurrentMenus);
	UFUNCTION()
	void ChangedSensValue(const float NewSensValue);
	void UIModes();
	void GameModes();
	
	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> CachedPauseMenu;
	UPROPERTY()
	TObjectPtr<UOptionMenuWidget> CachedOptionMenu;
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CachedCrosshairWidget;
	UPROPERTY()
	TObjectPtr<APawn> MyPawn;
	
	FInputModeGameAndUI UIMode;
	FInputModeGameOnly GameMode;
	
	bool bPauseMenu = false;
};
