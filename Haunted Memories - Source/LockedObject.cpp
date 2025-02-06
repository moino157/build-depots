// Fill out your copyright notice in the Description page of Project Settings.


#include "LockedObject.h"

#include "The_pacifiersCharacter.h"


void ALockedObject::EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::EnteringInteractionRange_Implementation(Interactor);
}

void ALockedObject::ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::ExitingInteractionRange_Implementation(Interactor);
}

void ALockedObject::Focused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Focused_Implementation(Interactor);
}

bool ALockedObject::Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item)
{
	IInteractibleInterface::Interact_Implementation(Interactor, Item);

	if(bIsUnlocked)
	{
		if(Item != nullptr) return false; // If the player tries to use an item on something that is unlocked
		Open();
		return true;
	}
	
	if(bAutoUseKey == true && Item == nullptr)
	{
		for(const FText RequiredKeyItemName : RequiredKeyItemNames)
		{
			if(const AKeyItem* required_item = Interactor->HasItem(RequiredKeyItemName); required_item != nullptr)
			{
				Unlock(Interactor, RequiredKeyItemName);
				Interactor->UseItem(RequiredKeyItemName);
				return true;
			}
		}
		
		Locked(Interactor);
		return false;
	}
	
	if(Item == nullptr){
		Locked(Interactor);
		return false;
	}
	
	for(const FText RequiredKeyItemName : RequiredKeyItemNames)
	{
		if(Item->ItemInformations.Name.EqualTo(RequiredKeyItemName))
		{
			Unlock(Interactor, RequiredKeyItemName);
			Interactor->UseItem(RequiredKeyItemName);
			return true;
		}
	}
	
	return false;
}

void ALockedObject::Unfocused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Unfocused_Implementation(Interactor);
}

// Sets default values
ALockedObject::ALockedObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add(FName("LockedObject"));
}

void ALockedObject::Locked_Implementation(AThe_pacifiersCharacter* Interactor)
{
}

void ALockedObject::Open_Implementation()
{
}

void ALockedObject::Unlock_Implementation(AThe_pacifiersCharacter* Interactor, const FText& RequiredKeyItemName)
{
}

// Called when the game starts or when spawned
void ALockedObject::BeginPlay()
{
	Super::BeginPlay();
	
}