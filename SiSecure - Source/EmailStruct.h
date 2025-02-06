// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmailStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FEmailStruct : public FTableRowBase
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EmailPrompt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool answer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EmailSender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SenderPicture;


};
