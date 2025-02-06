// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractibleInterface.generated.h"

class AHorrorCharacter;
// This class does not need to be modified.
UINTERFACE()
class UInteractibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTMANSION_API IInteractibleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool Interact(AHorrorCharacter* InteractingPlayer) = 0;
};
