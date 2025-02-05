// Fill out your copyright notice in the Description page of Project Settings.


#include "BMEndGameScreenWidget.h"

#include "Components/TextBlock.h"

void UBMEndGameScreenWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (WinnerTextBlock)
	{
		WinnerTextBlock->SetText(FText::FromString("Player # won"));
	}
}

void UBMEndGameScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (WinnerTextBlock)
	{
		WinnerTextBlock->SetText(FText::FromString("Player # won"));
	}
}
