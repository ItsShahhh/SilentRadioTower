// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget.h"

#include "DialogueData.h"
#include "WorldSystem.h"

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	WorldSystem = GetWorld()->GetSubsystem<UWorldSystem>();
	
	if (WorldSystem)
	{
		WorldSystem->OnProcessDialogue.AddDynamic(this, &UDialogueWidget::InitializeDialogue);
	}
}

void UDialogueWidget::InitializeDialogue(const FDialogueLine& DialogueData)
{
	if (DialogueData.SpeakerName.IsEmpty() || DialogueData.DialogueText.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("DialogueData Is Not Completed"));
		return;
	}
	
	//Enable Dialogue Widget Because We Make It Collapsed Inside Player Character When Added
	SetVisibility(ESlateVisibility::Visible);
	
	//Set All The Dialogue Data To This Everytime Starting The Dialogue
	NPCNameTextBlock->SetText(DialogueData.SpeakerName);
	DialogueSpeed = DialogueData.TextSpeed;
	DialogueArray = DialogueData.DialogueText;
	CurrentDialogueArrayIndex = 0;
	
	StartDialogue();
}

void UDialogueWidget::StartDialogue()
{
	//Whenever Dialogue Start We Need To Reset The Current Typed String and Current Dialogue Letter
	CurrentTypedString = TEXT("");
	CurrentDialogueLetter = 0;
	
	//When Start Dialogue It Will Check Which Array Of Dialogue String Will Be Used Based On Current Dialogue Array Index
	FullDialogueString = DialogueArray[CurrentDialogueArrayIndex];
	
	GetWorld()->GetTimerManager().SetTimer(DialogueHandleTimer, [this]
	{
		TypeChar();
	},
	DialogueSpeed,
	true
	);
}

void UDialogueWidget::TypeChar()
{
	if (CurrentDialogueLetter < FullDialogueString.Len())
	{
		CurrentTypedString.AppendChar(FullDialogueString[CurrentDialogueLetter]);
		DialogueTextBlock->SetText(FText::FromString(CurrentTypedString));
		CurrentDialogueLetter++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(DialogueHandleTimer);
		bFinishedDialogue = true;
	}
}

FReply UDialogueWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!bFinishedDialogue)
	{
		GetWorld()->GetTimerManager().ClearTimer(DialogueHandleTimer);
		DialogueTextBlock->SetText(FText::FromString(FullDialogueString));
		bFinishedDialogue = true;
	}
	else
	{
		//Add Dialogue Array Index (If Do Has Multiply Dialogue String)
		CurrentDialogueArrayIndex++;
		
		//Check The Current Number For Current Dialogue Array If Contains Inside The Array
		//If True We Start Dialogue Again With Current Dialogue Getting Increment
		//If False The Dialogue Is Going to Call Close Dialogue Function
		if (DialogueArray.IsValidIndex(CurrentDialogueArrayIndex))
		{
			StartDialogue();
		}
		else
		{
			CloseDialogue();
			return FReply::Handled().ClearUserFocus();
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDialogueWidget::CloseDialogue()
{
	//Set Dialogue To Visible Because It's Collapsed Inside Player Character
	SetVisibility(ESlateVisibility::Collapsed);
	
	//Broadcast To void Function Inside Player Character
	OnDisableDialogue.Broadcast();
}