// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmailSenderStruct.h"
#include "SignatureStruct.h"
#include "SenderProfileStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FSenderProfileStruct : public FTableRowBase
{
public:
	GENERATED_BODY()

	// Profile picture seen in the profile widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SenderProfilePicture;

	// Sender name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	// Sender email (user, doamine and TLD)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEmailSenderStruct SenderEmail;

	//Sender signature
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSignatureStruct Signature;
};
