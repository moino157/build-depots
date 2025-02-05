// Fill out your copyright notice in the Description page of Project Settings.


#include "CasseBriqueHUD.h"

#include "Components/TextBlock.h"

void UCasseBriqueHUD::NativeConstruct()
{
	Super::NativeConstruct();
	if (TB_Score)
	{
		TB_Score->SetText(FText::FromString("0"));
	}

	if (TB_Lives)
	{
		TB_Lives->SetText(FText::FromString("3"));
	}

	if (TB_OnScreenMessage)
	{
		TB_OnScreenMessage->SetText(FText::FromString(""));
	}
}

void UCasseBriqueHUD::SetScore(int32 new_score)
{
	if (TB_Score)
	{
		TB_Score->SetText(FText::FromString(FString::FromInt(new_score)));
	}
}

void UCasseBriqueHUD::SetLives(int32 new_bn_lives)
{
	if (TB_Lives)
	{
		TB_Lives->SetText(FText::FromString(FString::FromInt(new_bn_lives)));
	}
}

void UCasseBriqueHUD::SetOnScreenMessage(FString Message)
{
	if (TB_OnScreenMessage)
	{
		TB_OnScreenMessage->SetText(FText::FromString(Message));
	}
}
