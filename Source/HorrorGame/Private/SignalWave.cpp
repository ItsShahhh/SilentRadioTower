// Fill out your copyright notice in the Description page of Project Settings.


#include "SignalWave.h"

#include "EnhancedInputComponent.h"
#include "WorldSystem.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ASignalWave::ASignalWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	KnobMesh = CreateDefaultSubobject<UStaticMeshComponent>("KnobMesh");
	KnobMesh->SetupAttachment(MainMesh);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComp->SetupAttachment(MainMesh);
	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	ArrowComp->SetupAttachment(KnobMesh);
}

// Called when the game starts or when spawned
void ASignalWave::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASignalWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASignalWave::Interactable_Implementation(AActor* InteractActor)
{
	Super::Interactable_Implementation(InteractActor);
	
	if (!IsValid(InteractActor)) return;
    
	if (!PlayerRef && !PC)
	{
		PlayerRef = Cast<APawn>(InteractActor);
		if (PlayerRef)
		{
			PC = Cast<APlayerController>(PlayerRef->GetController());
		}
	}
    
	if (!IsValid(PC)) return;

	//Change Camera
	PC->SetViewTargetWithBlend(this, BlendSpeed, VTBlend_Linear);

	//SWAP INPUT (Freeze Player, Wake Up Machine)
	PlayerRef->DisableInput(PC);
	this->EnableInput(PC); // This creates this->InputComponent!
	
	//Now that EnableInput created the component, we cast it and bind.
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent))
	{
		// Clear previous binds so the knob doesn't spin twice as fast if you interact twice
		EnhancedInput->ClearActionEventBindings();
		EnhancedInput->BindAction(MouseRotateAction, ETriggerEvent::Triggered, this, &ASignalWave::RotateKnob);
	}
	
	//When Intreact It Randomize Knob Rotation Objective
	const float RandomRotation = FMath::RandRange(-90.f, 90.f);
	
	//Used FMath::TruncToFloat To Cut Off .00 Behind Float
	KnobRotationObjective = FMath::TruncToFloat(RandomRotation);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("%f"), KnobRotationObjective));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ASignalWave::RotateKnob(const FInputActionValue& Value)
{
	//Get 1DAxis From Mouse Input Action Value
	const float MouseDelta = Value.Get<float>();
	
	//We Add Mouse Value Multiply By The Rotation Into Mouse Knob Rotation
	//Reminder: We Use += Instead Of = Because = Will Overwrite The Knob Rotation and Its Only Going To be The Same Value
	//Remind: If We += It Added Up The Mouse Delta Value So It's Going Up Instead Overwrite The Same Number
	MouseKnobRotation += MouseDelta * KnobRotationSpeed;
	
	//Used FMath::TruncToFloat To Cut Off .00 Behind Float
	const float CleanKnobRotation = FMath::TruncToFloat(MouseKnobRotation);
	
	//Clamp So Knob Mesh Doesn't Rotate Beyond -90 and 90
	const float ClampKnobRotation = FMath::Clamp(CleanKnobRotation, -90.f, 90.f);
	
	//Used Set Relative Instead Of Add Local Because Add Local Just Add The Rotation And It Doesn't Know If Knob Already -90/90 Degrees
	KnobMesh->SetRelativeRotation(FRotator(0.0f, ClampKnobRotation, 0.0f));
	
	if (ClampKnobRotation == KnobRotationObjective)
	{
		//Set All Back To Default
		PC->SetViewTargetWithBlend(PlayerRef, BlendSpeed, VTBlend_Linear);
		this->DisableInput(PC);
		PlayerRef->EnableInput(PC);
		KnobMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		MouseKnobRotation = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Correct"));
		
		if (WorldSystem)
		{
			WorldSystem->CompletedQuest();
		}
	}
}
