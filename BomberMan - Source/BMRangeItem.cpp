// Fill out your copyright notice in the Description page of Project Settings.


#include "BMRangeItem.h"
#include "BomberManCharacter.h"

// Sets default values
ABMRangeItem::ABMRangeItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABMRangeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABMRangeItem::ApplyEffect(ABomberManCharacter* BMCharacter)
{
	BMCharacter->UpgradeBombRange();
	Destroy();
}

// Called every frame
void ABMRangeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

