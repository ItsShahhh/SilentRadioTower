// Fill out your copyright notice in the Description page of Project Settings.


#include "MorseCode.h"

#include "EnhancedInputComponent.h"
#include "GeneratorMachine.h"
#include "HorrorGamePlayerController.h"
#include "MorseCodeNumberWidget.h"
#include "Components/SpotLightComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AMorseCode::AMorseCode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MorseLight = CreateDefaultSubobject<USpotLightComponent>("MorseLight");
	MorseLight->SetupAttachment(MainMesh);
	
	LightQueueAudio = CreateDefaultSubobject<UAudioComponent>("LightQueueAudio");
	LightQueueAudio->SetupAttachment(MainMesh);
	
	MorseCodeLightAudio = CreateDefaultSubobject<UAudioComponent>("MorseCodeLightAudio");
	MorseCodeLightAudio->SetupAttachment(MainMesh);
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComp->SetupAttachment(MainMesh);
	
	LightButton = CreateDefaultSubobject<UStaticMeshComponent>("LightButton");
	LightButton->SetupAttachment(MainMesh);
	
	ConfirmButton = CreateDefaultSubobject<UStaticMeshComponent>("ConfirmButton");
	ConfirmButton->SetupAttachment(MainMesh);
	
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
	
	checkf(WidgetComp, TEXT("Failed To Get Widget Component In Morse Code"));
	
	MorseCodeNumberWidget = Cast<UMorseCodeNumberWidget>(WidgetComp->GetUserWidgetObject());
	
	checkf(MorseCodeNumberWidget, TEXT("Failed To Get Morse Code Number Widget In Morse Code"));
	
	RandomizeMorseCode();
}

// Called every frame
void AMorseCode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMorseCode::RandomizeMorseCode()
{
	for (int32 i = 0; i < CodeObjective.Num(); i++)
	{
		//Randomize Number For Each Interact
		CodeObjective[i] = FMath::RandRange(0,9);
	}
}

void AMorseCode::Interactable_Implementation(AActor* InteractActor)
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
			EnhancedInput->BindAction(ButtonInteractAction, ETriggerEvent::Started, this, &AMorseCode::OnMouseClicked);
		}
	}
}

void AMorseCode::OnMouseClicked()
{
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
		PressedButton(HitResult);
	}
}

void AMorseCode::PressedButton(const FHitResult& HitResult)
{
	const UPrimitiveComponent* PrimComp = HitResult.GetComponent();
	if (!PrimComp) return;
	
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
	else if (PrimComp == ConfirmButton)
	{
		CheckCorrectCode();
	}
	
	if (const int32* ButtonIndex = ButtonMap.Find(PrimComp))
	{
		UpdateDigit(*ButtonIndex);
	}
}

void AMorseCode::UpdateDigit(const int32 Index)
{
	if (!MorseCodeNumberWidget) return;
	if (!CodeNumbers.IsValidIndex(Index)) return;
	
	CodeNumbers[Index] = (CodeNumbers[Index] + 1) % 10;
	
	switch (Index)
	{
	case 0:
		MorseCodeNumberWidget->CodeNum1->SetText(FText::AsNumber(CodeNumbers[Index]));
		break;
					
	case 1:
		MorseCodeNumberWidget->CodeNum2->SetText(FText::AsNumber(CodeNumbers[Index]));
		break;
					
	case 2:
		MorseCodeNumberWidget->CodeNum3->SetText(FText::AsNumber(CodeNumbers[Index]));
		break;
				
	default:
		break;
	}
}


void AMorseCode::ActivateMorseLight()
{
	if (!CodeObjective.IsValidIndex(CurrentMorseCode)) return;
	
	//Use Find() to get a pointer instead of copying the struct
	const FMorseSequence* ChoosenMorseSignal = MorseSignalMap.Find(CodeObjective[CurrentMorseCode]);
	if (!ChoosenMorseSignal) return;

	if (!ChoosenMorseSignal->Signals.IsValidIndex(NextSignal))
	{
		//Increment The Current Morse Code After Finishing All The Light From Morse Code Before
		CurrentMorseCode++;
		
		//Reset Everything To Default
		NextSignal = 0;
		bIsMachineActive = false;
		bQueuesSound = true;
		
		//Check If We Already Activate All Light Based On Code Objective It Will Reset The Current Morse Code
		if (!CodeObjective.IsValidIndex(CurrentMorseCode))
		{
			CurrentMorseCode = 0;
		}
		
		//Only Reset To Default If All The Light Queues Sound Has Been Played
		if (CurrentLightQueue == LightQueueMap.Num())
		{
			CurrentLightQueue = 0;
		}
		
		return;
	}

	if (bQueuesSound)
	{
		//Use Find() to check existence and grab the sound simultaneously
		if (USoundBase** FoundSound = LightQueueMap.Find(CurrentLightQueue))
		{
			LightQueueAudio->SetSound(*FoundSound);
			LightQueueAudio->Play();
			bQueuesSound = false;
		}
	}
	
	//Set This To True For PressedButton Function So It Cannot Be Spammed
	bIsMachineActive = true;
	
	//Determine Beep Duration Based On Current Signal
	BeepDuration = ChoosenMorseSignal->Signals[NextSignal] == EMorseSignal::Dot ? ShortBeep : LongBeep;

	MorseLight->SetVisibility(true);
	
	GetWorldTimerManager().SetTimer(MorseLightTimerHandle, this, &AMorseCode::DeactivateMorseLight, BeepDuration, false);
}

void AMorseCode::DeactivateMorseLight()
{
	MorseLight->SetVisibility(false);
	NextSignal++;
	
	GetWorldTimerManager().SetTimer(MorseLightTimerHandle, this, &AMorseCode::ActivateMorseLight, BeepDuration, false);
}

void AMorseCode::CheckCorrectCode()
{
	//Empty Integer For Checking Correct Code
	int32 CorrCode = 0;
	
	for (int32 i = 0; i < CodeObjective.Num(); i++)
	{
		const int32 CheckCodeObj = CodeObjective[i];
		const int32 CheckCodeNum = CodeNumbers[i];
		
		if (CheckCodeNum == CheckCodeObj)
		{
			CorrCode++;
			
			if (CorrCode == CodeObjective.Num())
			{
				ExitFromInteractable();
			}
		}
	}
}
