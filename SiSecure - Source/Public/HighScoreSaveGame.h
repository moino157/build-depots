// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HighScoreSaveGame.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct CYBERSECURITE_API FHighScoreData
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Score;

    FHighScoreData() : PlayerName(TEXT("")), Score(0) {}

    FHighScoreData(FString NewPlayerName, int32 NewScore)
        : PlayerName(NewPlayerName), Score(NewScore) {}
};

UCLASS()
class CYBERSECURITE_API UHighScoreSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UHighScoreSaveGame();

    UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
    TArray<FHighScoreData> HighScores;

    // Static method to add a high score
    static void AddHighScore(const FString& PlayerName, int32 Score);

    // Static method to retrieve high scores
    static TArray<FHighScoreData> GetHighScores();

    // Static method to initialize first save table of game life
    static void WasHighScoresInitialized();

private:

    static TArray<FHighScoreData> GenerateDefaultHighScores();

};
