// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractibleObject.h"


void AInteractibleObject::EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::EnteringInteractionRange_Implementation(Interactor);
}

void AInteractibleObject::ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::ExitingInteractionRange_Implementation(Interactor);
}

void AInteractibleObject::Focused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Focused_Implementation(Interactor);
}

bool AInteractibleObject::Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item)
{
	IInteractibleInterface::Interact_Implementation(Interactor, Item);

	return true;
}

void AInteractibleObject::Unfocused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Unfocused_Implementation(Interactor);
}

// Sets default values
AInteractibleObject::AInteractibleObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractibleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

