// Fill out your copyright notice in the Description page of Project Settings.


#include "BMBox.h"

#include "BMMapGrid.h"
#include "BMMoreBombItem.h"
#include "BMRangeItem.h"
#include "BMShieldItem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABMBox::ABMBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique("Box");
}

// Called when the game starts or when spawned
void ABMBox::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABMMapGrid::StaticClass(), actors);
	if(actors.Num() > 0)
		MapRef = Cast<ABMMapGrid>(actors[0]);
	checkf(MapRef, TEXT("Bomberman Pawns are on a map without a Bomberman Grid."));
}

void ABMBox::BombHit()
{	
	PlayFX();
	ItemRoll();
	MapRef->RemovePawnFromGrid(this);
	Destroy();
}

void ABMBox::ItemRoll()
{
	// Spawn Item if luck roll succeed
	int roll = FMath::RandRange(0,100);
	if(roll >= 50)	// Roll succeeded (50%)
	{
		ABMItem* item_actor = nullptr;
		if(roll >= 90)	// Shield item (20%)
		{
			item_actor = GetWorld()->SpawnActor<ABMShieldItem>(ShieldItemClass, GetActorLocation(), FRotator(0,0,0));
			UE_LOG(LogTemp, Warning, TEXT("Shield Item [1] Spawned at %s"), *GetActorLocation().ToString());
		}else if(roll >= 70)	// More bombs item (40%)
		{
			item_actor = GetWorld()->SpawnActor<ABMMoreBombItem>(MoreBombsItemClass, GetActorLocation(), FRotator(0,0,0));
			UE_LOG(LogTemp, Warning, TEXT("More Bombs [2] Item Spawned at %s"), *GetActorLocation().ToString());
		}else  // Bomb Range Item (40%) 
		{
			item_actor = GetWorld()->SpawnActor<ABMRangeItem>(RangeItemClass, GetActorLocation(), FRotator(0,0,0));
			UE_LOG(LogTemp, Warning, TEXT("More Range [3] Item Spawned at %s"), *GetActorLocation().ToString());
		}
		MapRef->AddNewPawnToGrid(item_actor);
	}else
	{
		MapRef->RemovePawnFromGrid(this);
	}
}

void ABMBox::PlayFX()
{
	// Play FXs
    checkf(BreakingVFX, TEXT("Box VFX not set in blueprint"));
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
    	GetWorld(),
    	BreakingVFX,
    	GetActorLocation() + FVector(50,50,0)
    	);
    checkf(BreakingSFX, TEXT("Box breaking sound not set in blueprint"));
    UGameplayStatics::PlaySound2D(this, BreakingSFX);
}
