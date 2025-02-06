// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "Blueprint/UserWidget.h"
#include "TutoTools.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSECURITE_API UTutoTools : public UUserWidget
{
	GENERATED_BODY()

public : 
	FString password;
public :

	UFUNCTION(BlueprintCallable, Category = "Password")
	TArray<bool> GetPasswordStrength(const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "Password")
	void SavePassword(const FString& Password);

	UFUNCTION(BlueprintCallable, Category = "Password")
	bool IsEqualToOldPassword(const FString& Password);
};
