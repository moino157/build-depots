// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HyperlinkStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FHyperlinkStruct : public FTableRowBase
{
public:
	GENERATED_BODY()

	// "xxxxmoodle.uqac.ca/login"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Protocol;

	// "https://xxxxx.uqac.ca/login"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Subdomain;

	// "https://moodle.xxxxx.ca/login"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SDL;

	// "https://moodle.uqac.xxxxx/login"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TLD;

	// "https://moodle.uqac.ca/xxxxx"
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PagePath;
};
