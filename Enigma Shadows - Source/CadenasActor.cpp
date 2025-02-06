// Fill out your copyright notice in the Description page of Project Settings.


#include "CadenasActor.h"


// Sets default values
ACadenasActor::ACadenasActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACadenasActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACadenasActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACadenasActor::StopObserving() 
{
	Super::StopObserving();
	RemoveUI();
}

