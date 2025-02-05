// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CasseBriqueHUD.generated.h"

/**
 * 
 */
UCLASS()
class TPCASSEBRIQUE_API UCasseBriqueHUD : public UUserWidget
{
	GENERATED_BODY()

	int32 score_value;
	FText score_text;
	FText lives_text;
	FText on_screen_message;

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Score;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_Lives;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_OnScreenMessage;

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetScore(int32 new_score);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetLives(int32 new_bn_lives);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetOnScreenMessage(FString Message);
};
