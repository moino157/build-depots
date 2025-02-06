// Fill out your copyright notice in the Description page of Project Settings.


#include "BMBomb.h"

#include "BMMapGrid.h"
#include "BomberManCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABMBomb::ABMBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique("Bomb");
}

// Called when the game starts or when spawned
void ABMBomb::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(BombCountdown, this, &ABMBomb::OnBombCountdownOver, 3.0f);

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABMMapGrid::StaticClass(), actors);
	if(actors.Num() > 0)
		MapRef = Cast<ABMMapGrid>(actors[0]);
	checkf(MapRef, TEXT("Bomberman Pawns are on a map without a Bomberman Grid."));
}

void ABMBomb::BombHit()
{
	GetWorld()->GetTimerManager().ClearTimer(BombCountdown);
	GetWorld()->GetTimerManager().SetTimer(BombCountdown, this, &ABMBomb::OnBombCountdownOver, 0.1f);
}

void ABMBomb::OnBombCountdownOver()
{
	BombExplosion();
	GetWorld()->GetTimerManager().ClearTimer(BombCountdown);
}

void ABMBomb::BombExplosion()
{
	// Set fire at bomb's location
	TArray<int> coords = MapRef->GetCoords(GetActorLocation());
	MapRef->SpreadFireAtCoords(this, coords);
	PlaySFX();
	SpawnVFX(MapRef->CoordsToWorldVector(coords));
	for(int i = 0; i < 4; i++)	// Spread fire on all 4 sides
	{
		// Determine if fire is spreading up, down, left or right
		int direction_x = -1;
		int direction_y = 0;
		switch (i)
		{
		case 0:
			direction_x = 1;
			direction_y = 0;
			break;
		case 1:
			direction_x = 0;
			direction_y = -1;
			break;
		case 2:
			direction_x = 0;
			direction_y = 1;
			break;
		}
		
		for(int j = 1; j <= Range; j++)
		{
			if(!MapRef->SpreadFireAtCoords(this, {coords[0]+(direction_x*j), coords[1]+(direction_y*j)}))
				break;
		}
	}
	MapRef->RemovePawnFromGrid(this);
	Owner->CharacterBombExploded();
	Destroy();
}

void ABMBomb::PlaySFX()
{
	checkf(FireSFX, TEXT("No fire sound set in bomb blueprint."));
	UGameplayStatics::PlaySound2D(this, FireSFX);
}

void ABMBomb::SpawnVFX(FVector location)
{
	checkf(FireVFX, TEXT("No fire VFX set in bomb blueprint."));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		FireVFX,
		location + FVector(50,50,0)
		);
}

