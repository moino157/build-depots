// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HighScoreSaveGame.h"
#include "ScoreSubSystem.generated.h"

UENUM(BlueprintType)
enum class EScoreType : uint8 {
	EMAIL,
	PASSWORD,
	SHIELD,
	OTHER,
	
};

USTRUCT(BlueprintType)
struct CYBERSECURITE_API FPlayerScoreMileStone : public FTableRowBase
{
public:
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Score;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText Title;
	
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int, AddedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChangedWithType, int, AddedValue, EScoreType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTitleChanged, FText, Title);
/**
 * 
 */
UCLASS()


class CYBERSECURITE_API UScoreSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 GameScore = 0;
	FText CurrentTitle;

public:


	UFUNCTION(BlueprintCallable)
	void ModifyScore(int amount);

	UFUNCTION(BlueprintCallable)
	void ModifyScoreWithType(int amount, EScoreType type);

	UFUNCTION(BlueprintCallable)
	int GetScore();

	UFUNCTION(BlueprintCallable)
	void SaveScore();

	UFUNCTION(BlueprintCallable)
	float GetCurrentMilestonePercentage();

	UFUNCTION(BlueprintCallable)
	TArray<FHighScoreData> GetHighScores();

	UPROPERTY(BlueprintAssignable)
	FOnScoreChanged OnScoreChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnScoreChangedWithType OnScoreChangedWithTypeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTitleChanged OnTitleChangedDelegate;

	UFUNCTION(BlueprintCallable)
	FText GetTitleForScore();

	UFUNCTION(BlueprintCallable)
	FText GetDefaultTitle();

	UFUNCTION(BlueprintCallable)
	void SetTitle(FText title)
	{
		CurrentTitle = title;
	}


	UFUNCTION()
	void ResetSystem();
	
};
