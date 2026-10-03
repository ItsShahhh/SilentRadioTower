// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GettingPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UBTService_GettingPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;
	
	if (!Character)
	{
		Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}
	
	if (!IsValid(Character)) return;
	
	BBComp->SetValueAsVector(PlayerLocationKey.SelectedKeyName, Character->GetActorLocation());
}
