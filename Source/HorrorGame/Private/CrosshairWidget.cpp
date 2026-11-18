// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairWidget.h"

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCrosshairWidget::UpdateCrosshair(const bool bIsInteracting) const
{
	if (bIsInteracting)
	{
		InnerCircle->SetOpacity(1.0f);
	}
	else
	{
		InnerCircle->SetOpacity(0.0f);
	}
}