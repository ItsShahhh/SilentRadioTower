// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "QuestWidget.generated.h"

class UQuestSubSystem;
enum class ESettingMenus : uint8;
struct FQuestData;
class UWorldSystem;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UQuestSubSystem> QuestSubSystem;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDescription;
	
	UFUNCTION()
	void InitializeQuest(const FQuestData& QuestData) const;
	
};
