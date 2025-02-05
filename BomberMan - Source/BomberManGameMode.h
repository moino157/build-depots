// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BomberManGameMode.generated.h"

UCLASS(minimalapi)
class ABomberManGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABomberManGameMode();

protected:
	void SetupPlayers();
	virtual void BeginPlay() override;
	int GetNumberConnectedControllers();
public:
	
};



