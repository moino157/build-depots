// Fill out your copyright notice in the Description page of Project Settings.


#include "BMShieldItem.h"

#include "BomberManCharacter.h"


// Sets default values
ABMShieldItem::ABMShieldItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABMShieldItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABMShieldItem::ApplyEffect(ABomberManCharacter* BMCharacter)
{
	BMCharacter->UpgradeShield();
	Destroy();
}

// Called every frame
void ABMShieldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

