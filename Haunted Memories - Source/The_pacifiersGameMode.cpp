// Copyright Epic Games, Inc. All Rights Reserved.

#include "The_pacifiersGameMode.h"
#include "The_pacifiersCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThe_pacifiersGameMode::AThe_pacifiersGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
