// Fill out your copyright notice in the Description page of Project Settings.


#include "Generator.h"

#include "EnhancedInputComponent.h"
#include "WorldSystem.h"
#include "Camera/CameraComponent.h"

// Sets default values
AGenerator::AGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComp);
	
	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	GeneratorMesh->SetupAttachment(SceneComp);
	
	ValveMesh = CreateDefaultSubobject<UStaticMeshComponent>("ValveMesh");
	ValveMesh->SetupAttachment(GeneratorMesh);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(GeneratorMesh);
}

// Called when the game starts or when spawned
void AGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
}

// Called every frame
void AGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGenerator::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(InteractActor)) return;
	if (!IsValid(WorldSystem)) return;
	WorldSystem->OnUpdateLight.Broadcast();
	
	Pawn = Cast<APawn>(InteractActor);
	if (Pawn)
	{
		PC = Cast<APlayerController>(Pawn->GetController());
		if (PC)
		{
			PC->SetViewTargetWithBlend(this, BlendSpeed, VTBlend_Linear);
			
			//SWAP INPUT (Freeze Player, Wake Up Machine)
			Pawn->DisableInput(PC);
			this->EnableInput(PC); // This creates this->InputComponent!
	
			//Now that EnableInput created the component, we cast it and bind.
			if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent))
			{
				// Clear previous binds so the knob doesn't spin twice as fast if you interact twice
				EnhancedInput->ClearActionEventBindings();
				EnhancedInput->BindAction(ValveRotationAction, ETriggerEvent::Triggered, this, &AGenerator::RotateValve);
			}
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AGenerator::RotateValve(const FInputActionValue& Value)
{
	if (!IsValid(ValveMesh)) return;
	
	const float RotationValue = Value.Get<float>();
	ValveRotation += RotationValue * ValveRotationSpeed;
	const float ClampValveRotation = FMath::Clamp(ValveRotation, -180.0f, 180.0f);
	
	ValveMesh->SetRelativeRotation(FRotator(0.0f, 0.0f , ClampValveRotation));
	
	if (ValveMesh->GetComponentRotation().Equals(FRotator(0.0f, 0.0f, 180.0f), 0.1f))
	{
		ExitFromGenerator();
	}
}

void AGenerator::ExitFromGenerator()
{
	PC->SetViewTargetWithBlend(Pawn, BlendSpeed, VTBlend_Linear);
	
	Pawn->EnableInput(PC);
	this->DisableInput(PC);
}

