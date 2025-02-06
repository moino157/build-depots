// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultText.h"

void UDefaultText::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UDefaultText::NativeConstruct()
{
	Super::NativeConstruct();
	//SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UDefaultText::SetText(FText new_text)
{
	TextContent = new_text;
	UpdateDisplayedText();
}
