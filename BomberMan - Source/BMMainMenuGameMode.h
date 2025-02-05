// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BMMainMenuGameMode.generated.h"

class UBMMainMenuWidget;
/**
 * 
 */
UCLASS()
class BOMBERMAN_API ABMMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ABMMainMenuGameMode();
	UPROPERTY(BlueprintReadOnly, Category = "Game Mode");
	UBMMainMenuWidget* MainMenuWidget;
	UPROPERTY(BlueprintReadOnly, Category = "Game Mode");
	TArray<APlayerController*> PlayerControllers;
	void SetupPlayer();
	void AddNewController(APlayerController* controller);
	void RemoveUnusedControllers(int NumberOfPlayers);
};
