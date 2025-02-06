// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamemodeDifficultyStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct CYBERSECURITE_API FGamemodeDifficultyStruct
{
public:
	GENERATED_BODY()

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Percentages", meta=(ClampMin=0, ClampMax=100))
    int EasyDatabasePercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Percentages", meta = (ClampMin = 0, ClampMax = 100))
    int MediumDatabasePercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Percentages", meta = (ClampMin = 0, ClampMax = 100))
    int ImportantDatabasePercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty Percentages", meta = (ClampMin = 0, ClampMax = 100))
    int GrammarDestructionPercentage;

    //minimum number of elements set to this difficulty
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimum difficulty values", meta = (ClampMin = 0, ClampMax = 4))
    int EasyMinimumValues = 0;

    //minimum number of elements set to this difficulty
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimum difficulty values", meta = (ClampMin = 0, ClampMax = 4))
    int MediumMinimumValues = 0;

};
