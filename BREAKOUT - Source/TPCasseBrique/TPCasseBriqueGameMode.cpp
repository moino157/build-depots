// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPCasseBriqueGameMode.h"

#include "CasseBriqueBall.h"
#include "CasseBriqueHUD.h"
#include "TPCasseBriquePlayerController.h"
#include "TPCasseBriqueCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

ATPCasseBriqueGameMode::ATPCasseBriqueGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATPCasseBriquePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// Fetch Ball Class to use when respawning a ball
	static ConstructorHelpers::FClassFinder<AActor> BPClass(TEXT("/Game/Blueprint/BP_CasseBriqueBall"));
	if(BPClass.Class != NULL)
	{
		BallBP = BPClass;
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not fetch ball class"));
	}
}

void ATPCasseBriqueGameMode::OnPlayerDead()
{
	nb_live--;
	UCasseBriqueHUD* CBHUD = Cast<ATPCasseBriquePlayerController>(GetWorld()->GetFirstPlayerController())->HUDWidget;
	CBHUD->SetLives(nb_live);
	UE_LOG(LogTemp, Warning, TEXT("Lives: %i"), nb_live);
	if(nb_live > 0)
	{	// Respawn player
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATPCasseBriqueGameMode::BallRespawn, 3.0f, false);
	}else
	{	// Display GameOver screen and bring player back to main menu
		UE_LOG(LogTemp, Warning, TEXT("Game Over"));
		CBHUD->SetOnScreenMessage("PERDU");
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATPCasseBriqueGameMode::RestartLevel, 3.0f, false);
	}
}

void ATPCasseBriqueGameMode::TriggerWin()
{
	UE_LOG(LogTemp, Warning, TEXT("WIN CONDITION MET"));
	UCasseBriqueHUD* CBHUD = Cast<ATPCasseBriquePlayerController>(GetWorld()->GetFirstPlayerController())->HUDWidget;
	CBHUD->SetOnScreenMessage("VICTOIRE");
	StopBall();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATPCasseBriqueGameMode::RestartLevel, 3.0f, false);
}

void ATPCasseBriqueGameMode::RestartLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("Level Travel"));
	UGameplayStatics::OpenLevel(GetWorld(), FName(UGameplayStatics::GetCurrentLevelName(GetWorld(), false)));
}

void ATPCasseBriqueGameMode::StopBall()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACasseBriqueBall::StaticClass(), FoundActors);
	if(FoundActors.Num() > 0)
		Cast<ACasseBriqueBall>(FoundActors[0])->movement_speed = 0;
}

void ATPCasseBriqueGameMode::IncreaseScore(int32 added_score)
{
	UCasseBriqueHUD* CBHUD = Cast<ATPCasseBriquePlayerController>(GetWorld()->GetFirstPlayerController())->HUDWidget;
	score += added_score;
	CBHUD->SetScore(score);
}

void ATPCasseBriqueGameMode::BallRespawn()
{
	GetWorld()->SpawnActor(BallBP.Class);
}

void ATPCasseBriqueGameMode::GameOver()
{
}
