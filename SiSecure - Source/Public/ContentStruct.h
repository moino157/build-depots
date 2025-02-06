// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HyperlinkStruct.h"
#include "ContentStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FContentStruct : public FTableRowBase
{
public:
	GENERATED_BODY()

	// Object of the email
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Object;

	// Content of the email
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Content;

	// possibilty to add an image in the email content
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AdditionalImage;

	// Optional hyperlink
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHyperlinkStruct Hyperlink;
};
