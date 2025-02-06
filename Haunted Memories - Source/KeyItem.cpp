// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyItem.h"

#include "The_pacifiersCharacter.h"

void AKeyItem::EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::EnteringInteractionRange_Implementation(Interactor);
}

void AKeyItem::ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::ExitingInteractionRange_Implementation(Interactor);
}

void AKeyItem::Focused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Focused_Implementation(Interactor);
}

bool AKeyItem::Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item)
{
	if(bCanBePickedUp)
	{
		IInteractibleInterface::Interact_Implementation(Interactor, Item);
		Interactor->AddItem(this);
		Pickup_Implementation();
		return true;
	}
	CannotBePickedUp();
	return false;
}

void AKeyItem::Unfocused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Unfocused_Implementation(Interactor);
}

// Sets default values
AKeyItem::AKeyItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(FName("KeyItem"));
}

void AKeyItem::Pickup_Implementation()
{
	bHasBeenPickedUp = true;
	Pickup();
}

void AKeyItem::CannotBePickedUp_Implementation()
{
}

// Called when the game starts or when spawned
void AKeyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

