// Fill out your copyright notice in the Description page of Project Settings.


#include "BMMainMenuWidget.h"

#include "BomberMan/BMMainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"

void UBMMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBMMainMenuWidget::StartGamePressed(int number_of_players)
{
	StartGame(number_of_players);
}

void UBMMainMenuWidget::StartGame(int NumberOfPlayers)
{
	MenuGameMode->RemoveUnusedControllers(NumberOfPlayers);
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/TopDown/Maps/TopDownMap"), true, FString::Format(TEXT("{0}"), { NumberOfPlayers }));
}
