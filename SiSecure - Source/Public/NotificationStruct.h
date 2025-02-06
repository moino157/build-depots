// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NotificationStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FNotificationStruct
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NotificationPrompt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NotificationSender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SenderIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* NotificationSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor NotificationColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AdditionalImage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AdditionalImage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AdditionalImage3;
};
