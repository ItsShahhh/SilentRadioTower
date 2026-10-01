// Fill out your copyright notice in the Description page of Project Settings.


#include "Satelite.h"

#include "EnhancedInputComponent.h"
#include "WorldSystem.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASatelite::ASatelite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ValveMesh = CreateDefaultSubobject<UStaticMeshComponent>("ValveMesh");
	ValveMesh->SetupAttachment(RootComp);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ASatelite::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASatelite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASatelite::Interactable_Implementation(AActor* InteractActor)
{
	Super::Interactable_Implementation(InteractActor);
	
	if (!IsValid(InteractActor)) return;
	
	//Now that EnableInput created the component, we cast it and bind.
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent))
	{
		// Clear previous binds so the knob doesn't spin twice as fast if you interact twice
		EnhancedInput->ClearActionEventBindings();
		EnhancedInput->BindAction(ValveRotationAction, ETriggerEvent::Triggered, this, &ASatelite::RotateValve);
	}
	
	const float RandSatelliteRot = FMath::RandRange(-180.0f, 180.0f);
	SatelliteRotObj = FMath::TruncToFloat(RandSatelliteRot);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"), SatelliteRotObj));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASatelite::RotateValve(const FInputActionValue& Value)
{
	if (!IsValid(ValveMesh)) return;
	
	//Added and Multiply Rotation Value With Speed
	//Then Capped The Valve Rotation Using The Rotation Value That We Multiply and Added, Capped To Only -180 and 180
	const float RotationValue = Value.Get<float>();
	ValveRotation += RotationValue * ValveRotationSpeed;
	ValveRotation = FMath::Clamp(ValveRotation, -180.0f, 180.0f);
	
	//Multiply Valve Rotation By Rotation Value
	ValveMesh->SetRelativeRotation(FRotator(0.0f, 0.0f , ValveRotation));
	
	//Multiply The Current Rotation Satelite By Valve Rotation
	MainMesh->SetRelativeRotation(FRotator(0.0f, ValveRotation, 0.0f));
	
	if (ValveRotation == SatelliteRotObj)
	{
		ExitFromInteractable();
	}
}