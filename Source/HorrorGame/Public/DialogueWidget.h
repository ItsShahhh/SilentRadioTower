// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueData.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DialogueWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisableDialogue);

class UWorldSystem;
struct FDialogueLine;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NPCNameTextBlock;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UWorldSystem> WorldSystem;
	
	FDialogueLine DialogueDataRef;
	
	FString FullDialogueString;
	FString CurrentTypedString;
	TArray<FString> DialogueArray;
	
	int32 CurrentDialogueLetter;
	int32 CurrentDialogueArrayIndex;
	
	float DialogueSpeed;
	
	bool bFinishedDialogue;
	
	FTimerHandle DialogueHandleTimer;
	
	void TypeChar();
	void StartDialogue();
	UFUNCTION()
	void CloseDialogue();
	
public:
	FOnDisableDialogue OnDisableDialogue;
	
	UFUNCTION()
	void InitializeDialogue(const FDialogueLine& DialogueData);
	
};
