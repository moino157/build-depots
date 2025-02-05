// Fill out your copyright notice in the Description page of Project Settings.


#include "BMItem.h"


// Sets default values
ABMItem::ABMItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABMItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABMItem::ApplyEffect(ABomberManCharacter* BMCharacter)
{
	throw;	// This function needs to be overwritten by the child class
}
