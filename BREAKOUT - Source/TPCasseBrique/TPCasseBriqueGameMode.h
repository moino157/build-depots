// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPCasseBriqueGameMode.generated.h"

UCLASS(minimalapi)
class ATPCasseBriqueGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATPCasseBriqueGameMode();

	void OnPlayerDead();
	void TriggerWin();
	void IncreaseScore(int32 added_score);

private:
	void BallRespawn();
	void GameOver();
	void RestartLevel();
	void StopBall();

	FTimerHandle TimerHandle;
	ConstructorHelpers::FClassFinder<AActor> BallBP = NULL;
	int32 nb_live = 3;
	int32 score = 0;
};



