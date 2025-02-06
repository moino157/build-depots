// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractibleActor.h"

// Sets default values
AInteractibleActor::AInteractibleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInteractibleActor::BeginPlay()
{
	Super::BeginPlay();
}

bool AInteractibleActor::Interact(AHorrorCharacter* InteractingPlayer)
{
	Player = InteractingPlayer;
	return false;
}