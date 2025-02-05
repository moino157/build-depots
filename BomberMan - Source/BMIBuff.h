// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BMIBuff.generated.h"

class ABomberManCharacter;
// This class does not need to be modified.
UINTERFACE()
class UBMIBuff : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BOMBERMAN_API IBMIBuff
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyEffect(ABomberManCharacter* BMCharacter) = 0;
};
