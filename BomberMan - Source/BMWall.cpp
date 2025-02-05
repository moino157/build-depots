// Fill out your copyright notice in the Description page of Project Settings.


#include "BMWall.h"


// Sets default values
ABMWall::ABMWall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.AddUnique("Wall");
}

// Called when the game starts or when spawned
void ABMWall::BeginPlay()
{
	Super::BeginPlay();
	
}
