// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractibleInterface.generated.h"

class AThe_pacifiersCharacter;
class AKeyItem;
// This class does not need to be modified.
UINTERFACE()
class UInteractibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THE_PACIFIERS_API IInteractibleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	/** Called when the player enters the range where he can interact with the item */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnteringInteractionRange(AThe_pacifiersCharacter* Interactor);

	/** Called when the player exits the range where he can interact with the item */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExitingInteractionRange(AThe_pacifiersCharacter* Interactor);

	/** Called when the player looks at the item */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Focused(AThe_pacifiersCharacter* Interactor);

	/** Called when the player stops looking at the item */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Unfocused(AThe_pacifiersCharacter* Interactor);

	/** Called when the player interacts with the item */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Interact(AThe_pacifiersCharacter* Interactor, AKeyItem* Item = nullptr);
};
