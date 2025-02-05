// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BMMainMenuWidget.generated.h"

class ABMMainMenuGameMode;
/**
 * 
 */
UCLASS()
class BOMBERMAN_API UBMMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, Category = "Game Mode")
	ABMMainMenuGameMode* MenuGameMode;

	UFUNCTION(BlueprintCallable)
	void StartGamePressed(int number_of_players);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Mode")
	void UpdateReadyPlayers();

protected:
	void StartGame(int NumberOfPlayers);
};
