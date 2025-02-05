// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BMEndGameScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class BOMBERMAN_API UBMEndGameScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* WinnerTextBlock;
};
