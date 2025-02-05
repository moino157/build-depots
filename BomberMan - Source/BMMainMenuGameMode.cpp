// Fill out your copyright notice in the Description page of Project Settings.


#include "BMMainMenuGameMode.h"

#include "BMMenuController.h"
#include "BomberManPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/BMMainMenuWidget.h"

void ABMMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupPlayer();
}

ABMMainMenuGameMode::ABMMainMenuGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ABMMenuController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_MenuCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_MenuController"));
	if(PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UBMMainMenuWidget> MainMenuWidgetClass(TEXT("/Game/UI/WBP_MainMenu"));
	if(MainMenuWidgetClass.Succeeded()){
		MainMenuWidget = CreateWidget<UBMMainMenuWidget>(GetWorld(), MainMenuWidgetClass.Class);
		if (MainMenuWidget != nullptr)
		{
			MainMenuWidget->MenuGameMode = this;
			MainMenuWidget->AddToViewport();
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find main menu class."));
	}
	
}

void ABMMainMenuGameMode::SetupPlayer()
{
	// Get the first player controller (Player 0, which defaults to keyboard or the first controller)
	AddNewController(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	for(int i = 1; i < 4; i++)
	{
		APlayerController* PlayerController = UGameplayStatics::CreatePlayer(GetWorld(), i, true);
		UE_LOG(LogTemp, Warning, TEXT("Local player %d added"), i);
	}
}

void ABMMainMenuGameMode::AddNewController(APlayerController* controller)
{
	PlayerControllers.AddUnique(controller);
	MainMenuWidget->UpdateReadyPlayers();
	UE_LOG(LogTemp, Warning, TEXT("Added new player controller. Player id is %d. Total player count is now %d")
		, controller->GetLocalPlayer()->GetControllerId(), PlayerControllers.Num())
}

void ABMMainMenuGameMode::RemoveUnusedControllers(int NumberOfPlayers)
{
	const TArray<ULocalPlayer*>& LocalPlayers = GetWorld()->GetGameInstance()->GetLocalPlayers();
	TArray<ULocalPlayer*> players_to_remove;
	for(ULocalPlayer* player : LocalPlayers){
		if(player->GetControllerId() >= NumberOfPlayers)
			players_to_remove.Add(player);
	}
	for(ULocalPlayer* player: players_to_remove)
	{
		GetWorld()->GetGameInstance()->RemoveLocalPlayer(player);
	}
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController && PlayerController->GetLocalPlayer() && PlayerController->GetLocalPlayer()->GetControllerId() >= NumberOfPlayers)
		{
			PlayerController->Destroy();
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player %d kept."), PlayerController->GetLocalPlayer()->GetControllerId());
		}
	}
}
