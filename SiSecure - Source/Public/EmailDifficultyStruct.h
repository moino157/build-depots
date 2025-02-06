// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmailDifficultyStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FEmailDifficultyStruct
{
public:
	GENERATED_BODY()

	//Content data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ContentDataset;

	//Sender data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* SenderProfileDataset;


};
