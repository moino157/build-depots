// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DefaultText.generated.h"

/**
 * 
 */
UCLASS()
class TESTMANSION_API UDefaultText : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	FText TextContent;
public:
	UFUNCTION(BlueprintCallable)
	void SetText(FText new_text);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDisplayedText();

	
};
