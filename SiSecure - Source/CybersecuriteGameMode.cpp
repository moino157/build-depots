// Copyright Epic Games, Inc. All Rights Reserved.

#include "CybersecuriteGameMode.h"
#include "CybersecuriteCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACybersecuriteGameMode::ACybersecuriteGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
