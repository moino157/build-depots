// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BMMapGrid.generated.h"

class UBMEndGameScreenWidget;
class ABMBomb;
class ABomberManCharacter;

UCLASS()
class BOMBERMAN_API ABMMapGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABMMapGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<FTimerHandle> RespawnTimerHandle;
	FTimerHandle EndGameScreenTimerHandle;
	
	TArray<int> RespawnPlayerIDQueue;

	TSubclassOf<UBMEndGameScreenWidget> EndGameWidgetClass;

	int NumberOfPlayers = 4;

	void GetAllPawnCoords();

	int GetPawnOfClassCoords(const TSubclassOf<AActor>& SubclassOf);

	void EndGame();

	void LoadMainMenu();

	void printGrid();

public:
	TArray<TArray<TArray<AActor*>>> Pawns;	// 2D grid that can have multiple actors on a single tile

	TArray<int> GetCoords(FVector location);
	FVector CoordsToWorldVector(TArray<int> coords);

	static bool IsOutOfBounds(TArray<int> target_coords);

	bool CanGoOnTile(FVector target_loc);
	bool CanGoOnTile(FVector target_loc, AActor* ToIgnore);
	bool CanGoOnTile(TArray<int> target_coords);
	bool CanGoOnTile(TArray<int> target_coords, AActor* ToIgnore);

	bool Move(ABomberManCharacter* actor, TArray<int> target_loc);

	void AddNewPawnToGrid(AActor* BMPawn);
	void RemovePawnFromGrid(AActor* BMPawn);
	bool SpreadFireAtCoords(ABMBomb* Bomb, TArray<int> coords);

	void OnPlayerDie(ABomberManCharacter* BMPawn);
	void RespawnPlayer();
};
