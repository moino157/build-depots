// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmailSenderStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FEmailSenderStruct : public FTableRowBase
{
public:
	GENERATED_BODY()


	// first part of the email "xxxxx@gmail.com"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString uniqueName;

	// domain name "joe@xxxxx.com"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString domain;

	// ending of an email "joe@gmail.XXXX"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString topLevelDomain;




};
