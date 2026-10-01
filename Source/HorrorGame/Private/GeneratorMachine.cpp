// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorMachine.h"

#include "EnhancedInputComponent.h"
#include "WorldSystem.h"
#include "Camera/CameraComponent.h"

AGeneratorMachine::AGeneratorMachine()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(MainMesh);
	
	BigButton = CreateDefaultSubobject<UStaticMeshComponent>("BigButtonm");
	BigButton->SetupAttachment(MainMesh);
	
	SmallButton1 = CreateDefaultSubobject<UStaticMeshComponent>("SmallButton1");
	SmallButton1->SetupAttachment(MainMesh);
	
	SmallButton2 = CreateDefaultSubobject<UStaticMeshComponent>("SmallButton2");
	SmallButton2->SetupAttachment(MainMesh);
	
	SmallButton3 = CreateDefaultSubobject<UStaticMeshComponent>("SmallButton3");
	SmallButton3->SetupAttachment(MainMesh);
}

void AGeneratorMachine::Interactable_Implementation(AActor* InteractActor)
{
	Super::Interactable_Implementation(InteractActor);
	
	if (PC)
	{
		PC->bShowMouseCursor = true;
	}
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent))
	{
		if (ButtonInteractAction)
		{
			// Clear previous binds so the knob doesn't spin twice as fast if you interact twice
			EnhancedInput->ClearActionEventBindings();
			EnhancedInput->BindAction(ButtonInteractAction, ETriggerEvent::Started, this, &AGeneratorMachine::OnMouseClicked);
		}
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AGeneratorMachine::OnMouseClicked()
{
	if (!IsValid(PC)) return;
	
	FVector MouseLoc;
	FVector MouseDir;
	
	PC->DeprojectMousePositionToWorld(MouseLoc, MouseDir);
	
	const FVector StartTrace = MouseLoc;
	const FVector EndTrace = StartTrace + (MouseDir * LineTraceRange);
	FHitResult HitResult;
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_GameTraceChannel2);
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 2.0f, 0, 1.0f);
	
	if (bHit)
	{
		if (UPrimitiveComponent* HitComp = HitResult.GetComponent())
		{
			if (HitComp->ComponentHasTag("Button"))
			{
				if (PressedButtons.Contains(HitComp)) return;
			
				PressedButtons.Add(HitComp);
				
				if (PressedButtons.Num() == 4)
				{
					if (IsValid(WorldSystem))
					{
						WorldSystem->OnUpdateLight.Broadcast();
					}
					
					if (PC)
					{
						// Fixed Only Can Turn Camera Halfway Because Of Unlocking Mouse
						// Lock the invisible mouse back to the center of the screen!
						const FInputModeGameOnly GameOnlyMode;
						PC->SetInputMode(GameOnlyMode);
						
						PC->bShowMouseCursor = false;
					}
					ExitFromInteractable();
				}
			}
		}
	}
}
