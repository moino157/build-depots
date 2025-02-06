// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubSystem.h"
#include "GI_Cybersecurity.h"
#include <Kismet/GameplayStatics.h>

void UScoreSubSystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Score sub system initialized");
	
}

void UScoreSubSystem::ModifyScore(int amount)
{
	GameScore += amount;
	OnScoreChangedDelegate.Broadcast(amount);

	FText NewTitle = GetTitleForScore();
	if (!NewTitle.EqualTo(CurrentTitle))
	{
		CurrentTitle = NewTitle;
		OnTitleChangedDelegate.Broadcast(CurrentTitle);
	}
}

void UScoreSubSystem::ModifyScoreWithType(int amount, EScoreType type)
{
	GameScore += amount;
	OnScoreChangedWithTypeDelegate.Broadcast(amount, type);

	FText NewTitle = GetTitleForScore();
	if (!NewTitle.EqualTo(CurrentTitle))
	{
		CurrentTitle = NewTitle;
		OnTitleChangedDelegate.Broadcast(CurrentTitle);
	}
	
}

int UScoreSubSystem::GetScore()
{
	return GameScore;
}

void UScoreSubSystem::SaveScore()
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());

	if (gameInstancePtr)
	{
		UHighScoreSaveGame::AddHighScore(gameInstancePtr->PlayerName, GameScore);
	}

}

float UScoreSubSystem::GetCurrentMilestonePercentage()
{

	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());

	// Retrieve all rows from the DataTable
	TArray<FName> RowNames = gameInstancePtr->PlayerTitleMilestoneDataSet->GetRowNames();


	const FPlayerScoreMileStone* CurrentMilestone = nullptr;
	const FPlayerScoreMileStone* NextMilestone = nullptr;

	// Iterate through rows to find the current and next milestones
	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		const FPlayerScoreMileStone* Row = gameInstancePtr->PlayerTitleMilestoneDataSet->FindRow<FPlayerScoreMileStone>(
			RowNames[i],
			TEXT("GetCurrentMilestonePercentage"));

		if (!Row) continue;

		if (GameScore >= Row->Score)
		{
			CurrentMilestone = Row;
		}
		else
		{
			NextMilestone = Row;
			break;
		}
	}

	// If no current milestone, the player is below the first milestone
	if (!CurrentMilestone)
	{
		return 0.0f;
	}

	// If no next milestone, the player has completed all milestones
	if (!NextMilestone)
	{
		return 1.0f;
	}

	// Calculate percentage progress between current and next milestones
	int32 CurrentScore = CurrentMilestone->Score;
	int32 NextScore = NextMilestone->Score;

	if (NextScore == CurrentScore) // Prevent division by zero
		{
		return 1.0f;
		}

	return static_cast<float>(GameScore - CurrentScore) / static_cast<float>(NextScore - CurrentScore);
}

TArray<FHighScoreData> UScoreSubSystem::GetHighScores()
{
	return UHighScoreSaveGame::GetHighScores();
}

FText UScoreSubSystem::GetTitleForScore()
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());

	TArray<FPlayerScoreMileStone*> AllRows;
	gameInstancePtr->PlayerTitleMilestoneDataSet->GetAllRows(TEXT("Score Milestone Query"), AllRows);

	FText title;

	for (const FPlayerScoreMileStone* Row : AllRows)
	{
		if (Row && GameScore >= Row->Score)
		{
			title = Row->Title;
		}
	}

	if(title.IsEmpty())
	{
		title = GetDefaultTitle();
	}
	
	return title;
}

FText UScoreSubSystem::GetDefaultTitle()
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
	
	TArray<FName> RowNames = gameInstancePtr->PlayerTitleMilestoneDataSet->GetRowNames();
	const FPlayerScoreMileStone* FirstRow = gameInstancePtr->PlayerTitleMilestoneDataSet->FindRow<FPlayerScoreMileStone>(
	RowNames[0],
	TEXT("GetTitleForScore"));

	return FirstRow->Title;
};


void UScoreSubSystem::ResetSystem()
{
	GameScore = 0;
	CurrentTitle = GetDefaultTitle();
}
