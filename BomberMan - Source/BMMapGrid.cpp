// Fill out your copyright notice in the Description page of Project Settings.

#include "BMMapGrid.h"

#include "BMBomb.h"
#include "BMBox.h"
#include "BMWall.h"
#include "BomberManCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BMIDestroyable.h"
#include "BMMoreBombItem.h"
#include "BMRangeItem.h"
#include "BMShieldItem.h"
#include "Components/TextBlock.h"
#include "UI/BMEndGameScreenWidget.h"

// Sets default values
ABMMapGrid::ABMMapGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique("Grid");
	static ConstructorHelpers::FClassFinder<UBMEndGameScreenWidget> EndGameWidget(TEXT("/Game/UI/WBP_EndGameScreen"));
	if(EndGameWidget.Succeeded())
	{
		EndGameWidgetClass = EndGameWidget.Class;
	}
}

// Called when the game starts or when spawned
void ABMMapGrid::BeginPlay()
{
	Super::BeginPlay();
	GetAllPawnCoords();
	//RespawnTimerHandle.SetNum(4);
}

void ABMMapGrid::GetAllPawnCoords()
{
	// Initialize array to be empty
	Pawns.SetNum(11);
	for(int i = 0; i < Pawns.Num(); i++)
	{
		Pawns[i].SetNum(13);
	}

	GetPawnOfClassCoords(ABMBox::StaticClass());
	NumberOfPlayers = GetPawnOfClassCoords(ABomberManCharacter::StaticClass());
	GetPawnOfClassCoords(ABMWall::StaticClass());

	printGrid();
}

int ABMMapGrid::GetPawnOfClassCoords(const TSubclassOf<AActor>& SubclassOf)
{
	TArray<AActor*> BMPawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SubclassOf, BMPawns);
	for(AActor* BMPawn : BMPawns)
	{
		TArray<int> coords = GetCoords(BMPawn->GetActorLocation());
		if(IsOutOfBounds(coords))
			continue;
		
		if(Pawns[coords[0]][coords[1]].Num() == 0)	// If tile is empty
			Pawns[coords[0]][coords[1]].Add(BMPawn);
		else
			UE_LOG(LogTemp, Error, TEXT("There is already an actor in the (%d,%d) tile."), coords[0],coords[1]);
	}
	return BMPawns.Num();
}

void ABMMapGrid::EndGame()
{
	if(EndGameWidgetClass)
	{
		if (UBMEndGameScreenWidget* EndGameWidget = CreateWidget<UBMEndGameScreenWidget>(GetWorld(), EndGameWidgetClass))
		{
			EndGameWidget->AddToViewport();
			TArray<AActor*> BMPawns;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABomberManCharacter::StaticClass(), BMPawns);
			if(BMPawns.Num() > 0)
			{
				ABomberManCharacter* Player = Cast<ABomberManCharacter>(BMPawns[0]);
				// Create message string
				const FString FormatString = TEXT("Player {0} won");
				FText PlayerID = FText::AsNumber(Player->player_id+1);
				FText FormattedText = FText::Format(
					FText::FromString(FormatString), 
					PlayerID
				);
				
				EndGameWidget->WinnerTextBlock->SetText(FormattedText);
			}else
			{
				EndGameWidget->WinnerTextBlock->SetText(FText::FromString("Draw"));
			}
			
		}
	}
	GetWorld()->GetTimerManager().SetTimer(EndGameScreenTimerHandle, this, &ABMMapGrid::LoadMainMenu, 5.0f);
}

void ABMMapGrid::LoadMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Levels/LVL_MainMenu"));
}

FVector ABMMapGrid::CoordsToWorldVector(TArray<int> coords)
{
	FVector new_location = FVector(coords[1]*100, coords[0]*100, 96);
	return FVector(new_location.Y, new_location.X, 0);
}

void ABMMapGrid::printGrid()
{
	FString grid;
	grid.Append("Grid: \n");
	for(int i = Pawns.Num()-1; i >= 0; i--)
	{
		for(int j = 0; j < Pawns[i].Num(); j++)
		{
			grid.Append("[");
			
			if(Pawns[i][j].Num() == 0)
				grid.Append(" ");
			else
			{
				for(int k = 0; k < Pawns[i][j].Num(); k++)
				{
					if(Pawns[i][j][k]->IsA(ABomberManCharacter::StaticClass()))
						grid.Append("P");
					else if(Pawns[i][j][k]->IsA(ABMBox::StaticClass()))
						grid.Append("x");
					else if(Pawns[i][j][k]->IsA(ABMBomb::StaticClass()))
						grid.Append("O");
					else if(Pawns[i][j][k]->IsA(ABMWall::StaticClass()))
						grid.Append("-");
					else if(Pawns[i][j][k]->IsA(ABMShieldItem::StaticClass()))
						grid.Append("1");
					else if(Pawns[i][j][k]->IsA(ABMMoreBombItem::StaticClass()))
						grid.Append("2");
					else if(Pawns[i][j][k]->IsA(ABMRangeItem::StaticClass()))
						grid.Append("3");
				}
			}
			grid.Append("]");
		}
		grid.Append("\n");
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *grid);
}

void ABMMapGrid::RemovePawnFromGrid(AActor* BMPawn)
{
	// Remove Bomb from play grid
	TArray<int> coords = GetCoords(BMPawn->GetActorLocation());
	for(int i = 0; i < Pawns[coords[0]][coords[1]].Num(); i++)
	{
		if(Pawns[coords[0]][coords[1]][i] == BMPawn)
		{
			Pawns[coords[0]][coords[1]].RemoveAt(i);
			return;
		}
	}
}

void ABMMapGrid::AddNewPawnToGrid(AActor* BMPawn)
{
	TArray<int> coords = GetCoords(BMPawn->GetActorLocation());
	Pawns[coords[0]][coords[1]].Add(BMPawn);
	UE_LOG(LogTemp, Warning, TEXT("Bomb placed on tile (%d,%d)"), coords[0], coords[1]);
	printGrid();
}

/**
 * Applies damage
 * @param coords Coordinates where the fire is located
 * @return returns true if fire can continue to spread; otherwise false
 */
bool ABMMapGrid::SpreadFireAtCoords(ABMBomb* Bomb, TArray<int> coords)
{
	if(IsOutOfBounds(coords))
		return false;
	bool spread_fire = true;
	// If tile is not empty
	if(Pawns[coords[0]][coords[1]].Num() > 0)
	{
		for(int i = 0; i < Pawns[coords[0]][coords[1]].Num(); i++)
		{
			// If the tile is occupied by a wall, nothing is done
			if(Pawns[coords[0]][coords[1]][i]->Tags.Contains("Wall"))
				return false;
			Bomb->SpawnVFX(CoordsToWorldVector(coords));
			// Apply damage if applicable
			if(Pawns[coords[0]][coords[1]][i]->Implements<UBMIDestroyable>())
			{
				const TArray<FName> ActorTags = Pawns[coords[0]][coords[1]][i]->Tags;
				IBMIDestroyable* DestroyableInterfaceInstance = Cast<IBMIDestroyable>(Pawns[coords[0]][coords[1]][i]);
				checkf(DestroyableInterfaceInstance, TEXT("Actor implementing UBMIDestroyable couldn't be casted to IBMIDestroyable???"));
				if (!Pawns[coords[0]][coords[1]][i]->IsPendingKillPending())
					DestroyableInterfaceInstance->BombHit();
				if(spread_fire && ActorTags.Contains("Box") || ActorTags.Contains("Bomb"))
					spread_fire = false;
			}
		}
	}else
	{
		Bomb->SpawnVFX(CoordsToWorldVector(coords));
	}
	return spread_fire;
}

void ABMMapGrid::OnPlayerDie(ABomberManCharacter* BMPawn)
{
	/*
	// Create respawn timer
	UE_LOG(LogTemp, Warning, TEXT("Player %s died. Respawning in 2 secondes."), *BMPawn->GetActorLabel());
	// Send player id in respawn function parameter
	RespawnPlayerIDQueue.Add(BMPawn->player_id);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle[BMPawn->player_id], this, &ABMMapGrid::RespawnPlayer, 3.0f);
	*/
	if(--NumberOfPlayers == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(EndGameScreenTimerHandle, this, &ABMMapGrid::EndGame, 0.1f);
	}
}

void ABMMapGrid::RespawnPlayer()
{
	const int player_id = RespawnPlayerIDQueue.Pop();
	UGameplayStatics::CreatePlayer(GetWorld(), player_id, true);
}

TArray<int> ABMMapGrid::GetCoords(FVector location)
{
	const int x_coord = FMath::Floor(location.X/100);
	const int y_coord = FMath::Floor(location.Y/100);
	TArray<int> coords = {-1,-1};
	if(x_coord >= Pawns.Num() || y_coord >= Pawns[x_coord].Num() || x_coord < 0 || y_coord < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("GetCoords: (%d,%d) is out of bound. "), x_coord, y_coord);
	}
	else
	{
		coords[0] = x_coord;
		coords[1] = y_coord;
	}
	return coords;
}

bool ABMMapGrid::IsOutOfBounds(TArray<int> target_coords)
{
	return target_coords[0] < 0 || target_coords[1] < 0 || target_coords[0] > 10 || target_coords[1] > 12;
}

bool ABMMapGrid::CanGoOnTile(FVector target_loc)
{
	TArray<int> coords = GetCoords(target_loc);
	return CanGoOnTile(coords);
}

bool ABMMapGrid::CanGoOnTile(FVector target_loc, AActor* ToIgnore)
{
	TArray<int> coords = GetCoords(target_loc);
	return CanGoOnTile(coords, ToIgnore);
}

bool ABMMapGrid::CanGoOnTile(TArray<int> target_coords)
{
	// Returns true if not out of bounds, tile is not occupied and if so, if the actor occupying the tile is not an obstacle
	if(IsOutOfBounds(target_coords))
		return false;
	for(int i = 0; i < Pawns[target_coords[0]][target_coords[1]].Num() ; i++)
	{
		if(Pawns[target_coords[0]][target_coords[1]][i] != nullptr && Pawns[target_coords[0]][target_coords[1]][i]->Implements<UBMIObstacle>())
			return false;
	}
	
	return true;
}

/**
 * Verify if a BMPawn could be placed on the tile.
 * @param target_coords Tile to verify
 * @param ToIgnore Actor to ignore
* @return true if not out of bounds, tile is not occupied and if so, if the actor occupying the tile is not an obstacle
 * nor the actor to ignore specified in the parameters; otherwise false
 */
bool ABMMapGrid::CanGoOnTile(TArray<int> target_coords, AActor* ToIgnore)
{
	if(IsOutOfBounds(target_coords))
		return false;
	for(int i = 0; i < Pawns[target_coords[0]][target_coords[1]].Num() ; i++)
	{
		if((Pawns[target_coords[0]][target_coords[1]][i] != nullptr && Pawns[target_coords[0]][target_coords[1]][i]->Implements<UBMIObstacle>())
		&& Pawns[target_coords[0]][target_coords[1]][i] != ToIgnore)
		{
			//UE_LOG(LogTemp, Error, TEXT("Actor %s can't move to tile (%d, %d) because it's occupied by actor %s")
			//	, *ToIgnore->GetActorLabel(), target_coords[0], target_coords[1], *Pawns[target_coords[0]][target_coords[1]][i]->GetActorLabel());
			return false;
		}
	}
	return true;
}

bool ABMMapGrid::Move(ABomberManCharacter* Character, TArray<int> movement)
{
	UE_LOG(LogTemp, Warning, TEXT("Movement Vector Provided is (%d, %d)"), movement[0], movement[1]);
	TArray<int> current_coords = GetCoords(Character->GetActorLocation());
	TArray<int> target_coords = { current_coords[0] + movement[0], current_coords[1] + movement[1] };
	if(CanGoOnTile(target_coords) == false)
	{
		if(!IsOutOfBounds(target_coords))
		{
			//UE_LOG(LogTemp, Error, TEXT("Actor is on tile (%d, %d) and cant move by (%d, %d) to tile (%d, %d) because of actor %s")
			//, current_coords[0], current_coords[1], movement[0], movement[1], target_coords[0], target_coords[1]
			//, *Pawns[target_coords[0]][target_coords[1]].Last()->GetActorLabel());
		}
		return false;
	}
	RemovePawnFromGrid(Character);
	
	for(int i = 0; i < Pawns[target_coords[0]][target_coords[1]].Num(); i++)
	{
		if(ABMItem* item = Cast<ABMItem>(Pawns[target_coords[0]][target_coords[1]][i]))	// Handle item on tiles
		{
			item->ApplyEffect(Character);
			RemovePawnFromGrid(item);
		}
	}
	
	Pawns[target_coords[0]][target_coords[1]].Add(Character);
	Character->SetActorLocation(CoordsToWorldVector(target_coords) + FVector(50,50,96));
	printGrid();
	return true;
}
