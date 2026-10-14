// Fill out your copyright notice in the Description page of Project Settings.


#include "MorseCode.h"

#include "EnhancedInputComponent.h"
#include "GeneratorMachine.h"
#include "MorseCodeNumberWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"

// Sets default values
AMorseCode::AMorseCode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MorseLight = CreateDefaultSubobject<USpotLightComponent>("MorseLight");
	MorseLight->SetupAttachment(MainMesh);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(MainMesh);
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComp->SetupAttachment(MainMesh);
	
	LightButton = CreateDefaultSubobject<UStaticMeshComponent>("LightButton");
	LightButton->SetupAttachment(MainMesh);
	
	Button1 = CreateDefaultSubobject<UStaticMeshComponent>("Button1");
	Button1->SetupAttachment(MainMesh);
	
	Button2 = CreateDefaultSubobject<UStaticMeshComponent>("Button2");
	Button2->SetupAttachment(MainMesh);
	
	Button3 = CreateDefaultSubobject<UStaticMeshComponent>("Button3");
	Button3->SetupAttachment(MainMesh);
}

// Called when the game starts or when spawned
void AMorseCode::BeginPlay()
{
	Super::BeginPlay();
	
	ButtonMap.Add(Button1,0);
	ButtonMap.Add(Button2,1);
	ButtonMap.Add(Button3, 2);
	
	CorrectButtonMap.Add(0, false);
	CorrectButtonMap.Add(1, false);
	CorrectButtonMap.Add(2, false);
	
	if (WidgetComp)
	{
		MorseCodeNumberWidget = Cast<UMorseCodeNumberWidget>(WidgetComp->GetUserWidgetObject());
	}
	
	RandomizeMorseCode();
}

// Called every frame
void AMorseCode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMorseCode::RandomizeMorseCode()
{
	for (int32 i = 0; i < 3; i++)
	{
		//Check If The Index On The Array Is Already Added
		//If Not It Will Add New Index and New Morse Code For That Index
		if (!CodeObjective.IsValidIndex(i))
		{
			//Randomize Number For Each Interact
			const int32 RandNumber = FMath::RandRange(0,9);
		
			CodeObjective.Add(RandNumber);
		}
	}
}

void AMorseCode::Interactable_Implementation(AActor* InteractActor)
{
	if (!IsValid(MorseLight)) return;
	if (MorseSignalMap.IsEmpty()) return;
	if (CodeObjective.IsEmpty()) return;
	
	Super::Interactable_Implementation(InteractActor);
	
	if (PC)
	{
		PC->bShowMouseCursor = true;
		
		//Set Input To Game And UI, So When Interact Again It Will Be Able To Click
		const FInputModeGameAndUI GameAndUIMode;
		PC->SetInputMode(GameAndUIMode);
	}
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(this->InputComponent))
	{
		if (ButtonInteractAction)
		{
			// Clear previous binds so the knob doesn't spin twice as fast if you interact twice
			EnhancedInput->ClearActionEventBindings();
			EnhancedInput->BindAction(ButtonInteractAction, ETriggerEvent::Started, this, &AMorseCode::OnMouseClicked);
		}
	}
}

void AMorseCode::ActivateMorseLight()
{
	//Getting The Current Code Objective Need For Button
	const int32 CodeObjectiveIndex = CodeObjective[CurrentMorseCode];
	
	//Based On The Random Number It Chose The Morse Signal Map
	ChoosenMorseSignal = MorseSignalMap[CodeObjectiveIndex];
	
	if (!ChoosenMorseSignal.Signals.IsValidIndex(NextSignal))
	{
		//If Next Signal Is More Than Array That Mean The Morse Code Is Only That Much
		//After That We Reset The Number For The Next Morse Code
		NextSignal = 0;
		bIsMachineActive = false;
		return;
	}
	
	//Set The bIsMachineActive To True Indicate Machine Is Current Active
	bIsMachineActive = true;

	//Crate Empty Float
	float BeepDuration = 0;
	
	//It Will Set The Beep Duration Based On The Signals That Been Chose By The Next Signal Index
	//Reminder: Can Just Used Ternary But Make It Like So It Easier To Understand 
	if (ChoosenMorseSignal.Signals[NextSignal] == EMorseSignal::Dot)
	{
		BeepDuration = ShortBeep;
	}
	else if (ChoosenMorseSignal.Signals[NextSignal] == EMorseSignal::Dash)
	{
		BeepDuration = LongBeep;
	}
	
	//Activate The Light
	MorseLight->SetVisibility(true);
	
	//Added Beep Sound Here
	
	GetWorldTimerManager().SetTimer(MorseLightTimerHandle, [this]
	{
		MorseLight->SetVisibility(false);
		NextSignal++;
		
		//Stop Beep Sound Here
		
		GetWorldTimerManager().SetTimer(MorseLightTimerHandle, [this]
		{
			ActivateMorseLight();
		},
		1.0f,
		false
		);
	},
	BeepDuration,
	false
	);
}

void AMorseCode::OnMouseClicked()
{
	if (!IsValid(PC)) return;
	if (!IsValid(MorseCodeNumberWidget)) return;
	if (ButtonMap.IsEmpty()) return;
	if (CodeNumbers.IsEmpty()) return;
	
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
		if (UPrimitiveComponent* PrimComp = HitResult.GetComponent())
		{
			//Activating Morse Light Code
			if (PrimComp == LightButton)
			{
				if (!bIsMachineActive)
				{
					ActivateMorseLight();
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("The Machine Is Already Active"));
				}
			}
			else if (const int32* ButtonIndex = ButtonMap.Find(PrimComp))
			{
				const int32 Index = *ButtonIndex;
				CodeNumbers[Index] = (CodeNumbers[Index] + 1) % 10;
				const int32 ButtonValue = CodeNumbers[Index];
				
				switch (Index)
				{
					case 0:
					MorseCodeNumberWidget->CodeNum1->SetText(FText::AsNumber(ButtonValue));
					break;
					
					case 1:
					MorseCodeNumberWidget->CodeNum2->SetText(FText::AsNumber(ButtonValue));
					break;
					
					case 2:
					MorseCodeNumberWidget->CodeNum3->SetText(FText::AsNumber(ButtonValue));
					break;
				
					default:
					break;
				}
				
				//Check Correct Code Everytime Click Button
				CheckCorrectCode(ButtonValue, Index);
			}
		}
	}
}

void AMorseCode::CheckCorrectCode(const int32 ButtonValue, const int32 Index)
{
	if (CodeObjective[Index] == ButtonValue)
	{
		CorrectButtonMap[Index] = true;
	}
	else
	{
		CorrectButtonMap[Index] = false;
	}
	
	//We start by assuming the puzzle is solved.
	bool bIsPuzzleSolved = true;

	//This loop automatically goes through every single row in the TMap one by one.
	for (const auto& Elem : CorrectButtonMap)
	{
		//Elem.Key gives you the UPrimitiveComponent*
		//Elem.Value gives you the data on the right side (e.g., the bool or int32)
		if (Elem.Value == false) 
		{
			bIsPuzzleSolved = false;
			break; 
		}
	}
	
	if (bIsPuzzleSolved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Completed"));
	}
}
