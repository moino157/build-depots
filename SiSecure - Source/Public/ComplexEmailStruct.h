// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HyperlinkStruct.h"
#include "SenderProfileStruct.h"
#include "ContentStruct.h"
#include "SignatureStruct.h"
#include "ComplexEmailStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FComplexEmailStruct : public FTableRowBase
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FContentStruct Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Salutation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSenderProfileStruct SenderInformation;

	// true = not a scam / false = scam
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsARealEmail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLinkedClicked = false;
	
};
