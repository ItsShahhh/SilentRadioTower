// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorMachine.h"
#include "EnhancedInputComponent.h"
#include "RandomEvent.h"
#include "WorldSystem.h"

AGeneratorMachine::AGeneratorMachine()
{
	BigButton = CreateDefaultSubobject<UStaticMeshComponent>("BigButtonm");
	BigButton->SetupAttachment(MainMesh);
	
	SmallButton1 = CreateDefaultSubobject<UStaticMeshComponent>("SmallButton1");
	SmallButton1->SetupAttachment(MainMesh);
	
	SmallButton2 = CreateDefaultSubobject<UStaticMeshComponent>("SmallButton2");
	SmallButton2->SetupAttachment(MainMesh);
	
	SmallButton3 = CreateDefaultSubobject<UStaticMeshComponent>("SmallButton3");
	SmallButton3->SetupAttachment(MainMesh);
}

void AGeneratorMachine::BeginPlay()
{
	Super::BeginPlay();
	
	ButtonsMap.Add(BigButton, false);
	ButtonsMap.Add(SmallButton1, false);
	ButtonsMap.Add(SmallButton2, false);
	ButtonsMap.Add(SmallButton3, false);
	
	WorldSystem->OnUpdateRandomEvent.AddDynamic(this, &AGeneratorMachine::RandGeneratorPower);
}

void AGeneratorMachine::Interactable_Implementation(AActor* InteractActor)
{
	Super::Interactable_Implementation(InteractActor);
	
	PC->bShowMouseCursor = true;
		
	//Set Input To Game And UI, So When Interact Again It Will Be Able To Click
	const FInputModeGameAndUI GameAndUIMode;
	PC->SetInputMode(GameAndUIMode);
	
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
	if (!bCanPressed) return;
	
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
		CheckButtons(HitResult);
	}
}

void AGeneratorMachine::CheckButtons(const FHitResult& HitResult)
{
	if (UPrimitiveComponent* HitComp = HitResult.GetComponent())
	{
		if (!ButtonsMap.Contains(HitComp)) return;
		
		//If Click Button That Already Pressed It Will Just Return Instead Of Checking
		if (ButtonsMap[HitComp]) return;
		
		if (bool* Pressed = ButtonsMap.Find(HitComp))
		{
			*Pressed = true;
		}
		
		bool bAlreadyPressed = true;
		
		for (const auto& Row : ButtonsMap)
		{
			if (Row.Value == false)
			{
				bAlreadyPressed = false;
			}
		}
		
		if (bAlreadyPressed)
		{
			//Reset The Button TMap To Be Used Another Time
			ButtonsMap.Empty();
			
			WorldSystem->InitiateRandomEvent();
			WorldSystem->OnUpdateLight.Broadcast(true);
			
			ExitFromInteractable();
		}
	}
}

void AGeneratorMachine::RandGeneratorPower(const ERandomEvent SelRanEvents)
{
	if (SelRanEvents == ERandomEvent::Generator)
	{
		bCanPressed = true;
	}
}
