// Fill out your copyright notice in the Description page of Project Settings.

#include "HighScoreSaveGame.h"
#include "Kismet/GameplayStatics.h"


UHighScoreSaveGame::UHighScoreSaveGame()
{
}

void UHighScoreSaveGame::AddHighScore(const FString& PlayerName, int32 Score)
{
    // Create or load the existing high score save game
    UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot("HighScoreSlot", 0));
    if (!SaveGameInstance)
    {
        SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));
    }

    // Add the new high score
    SaveGameInstance->HighScores.Add(FHighScoreData(PlayerName, Score));

    SaveGameInstance->HighScores.Sort([](const FHighScoreData& A, const FHighScoreData& B)
        {
            return A.Score > B.Score;
        });

    // Save the updated high scores back to the same slot
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, "HighScoreSlot", 0);
}

TArray<FHighScoreData> UHighScoreSaveGame::GetHighScores()
{
    TArray<FHighScoreData> HighScores;

    UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::LoadGameFromSlot("HighScoreSlot", 0));
    if (SaveGameInstance)
    {
        HighScores = SaveGameInstance->HighScores;
    }

    return HighScores;
}

void UHighScoreSaveGame::WasHighScoresInitialized()
{
    if (!UGameplayStatics::DoesSaveGameExist("HighScoreSlot", 0))
    {
        UHighScoreSaveGame* SaveGameInstance = Cast<UHighScoreSaveGame>(UGameplayStatics::CreateSaveGameObject(UHighScoreSaveGame::StaticClass()));

        // Populate with default high scores
        SaveGameInstance->HighScores = GenerateDefaultHighScores();

        // Save the new instance with the default high scores
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, "HighScoreSlot", 0);
    }
}

TArray<FHighScoreData> UHighScoreSaveGame::GenerateDefaultHighScores()
{
    TArray<FHighScoreData> DefaultHighScores;
    // Populate default high scores here
    DefaultHighScores.Add(FHighScoreData(TEXT("Kyle"), 0));
    DefaultHighScores.Add(FHighScoreData(TEXT("Sonia"), 2500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Erik"), 5000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Mina"), 7500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Noah"), 10000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Tara"), 12500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Liam"), 15000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Juno"), 17500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Finn"), 20000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Cleo"), 22500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Ruby"), 25000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Oscar"), 27500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Lucy"), 30000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Max"), 32500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Ella"), 35000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Nico"), 37500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Zara"), 40000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Milo"), 42500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Emma"), 45000));
    DefaultHighScores.Add(FHighScoreData(TEXT("Ivy"), 47500));
    DefaultHighScores.Add(FHighScoreData(TEXT("Alex"), 50000));

    DefaultHighScores.Sort([](const FHighScoreData& A, const FHighScoreData& B)
        {
            return A.Score > B.Score; // Ensure highest scores are first
        });

    return DefaultHighScores;
}
