// Copyright Epic Games, Inc. All Rights Reserved.

#include "BomberManGameMode.h"
#include "BomberManPlayerController.h"
#include "BomberManCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABomberManGameMode::ABomberManGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABomberManPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ABomberManGameMode::SetupPlayers()
{
	for(int i = 0; i < GetNumberConnectedControllers(); i++){
		UGameplayStatics::CreatePlayer(GetWorld(), i, true);
		UE_LOG(LogTemp, Warning, TEXT("Added player %d"), i);
	}
}

void ABomberManGameMode::BeginPlay()
{
	Super::BeginPlay();
	//SetupPlayers();
}

int ABomberManGameMode::GetNumberConnectedControllers()
{
	FString Options = GetWorld()->GetAddressURL();
	if(Options.Len() <= 0)
	{
		FString Option = UGameplayStatics::ParseOption(Options, TEXT("NbPlayers"));
		UE_LOG(LogTemp, Warning, TEXT("Option is %s"), *Option);
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get current level options"));
	}
	return 0;
}
