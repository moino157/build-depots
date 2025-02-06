// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleActor.h"

#include "Components/PuzzleComponent.h"
#include "HorrorCharacter.h"


// Sets default values
APuzzleActor::APuzzleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool APuzzleActor::Interact(AHorrorCharacter* InteractingPlayer)
{
	Super::Interact(InteractingPlayer);
	Component = NewObject<UPuzzleComponent>(Cast<AActor>(Player), UPuzzleComponent::StaticClass());
	Component->RegisterComponent();
	Component->SetupPlayerInputAction(MappingContext, IA_DirectionalInput, IA_Cancel);
	Component->BindToPuzzle(this);
	StartPuzzle();
	return true;
}

void APuzzleActor::Directional(FVector2d input)
{
	TurnHandle(input.X);
}

void APuzzleActor::PuzzleCompleted()
{
	Component->Back();
}

// Called when the game starts or when spawned
void APuzzleActor::BeginPlay()
{
	Super::BeginPlay();
}


