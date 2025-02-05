// Fill out your copyright notice in the Description page of Project Settings.


#include "BMMoreBombItem.h"

#include "BomberManCharacter.h"


// Sets default values
ABMMoreBombItem::ABMMoreBombItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABMMoreBombItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABMMoreBombItem::ApplyEffect(ABomberManCharacter* BMCharacter)
{
	BMCharacter->UpgradeBombNumber();
	Destroy();
}

// Called every frame
void ABMMoreBombItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

