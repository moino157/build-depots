// Fill out your copyright notice in the Description page of Project Settings.


#include "BMMenuController.h"

#include "BMMainMenuGameMode.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABMMenuController::ABMMenuController()
{
	
}

void ABMMenuController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);  // Optional: Mouse behavior
	SetInputMode(InputMode);
	UE_LOG(LogTemp, Warning, TEXT("Menu Controller detected"));
}

void ABMMenuController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Confirm, ETriggerEvent::Completed, this, &ABMMenuController::AddNewPlayer);
	}
}

void ABMMenuController::AddNewPlayer(const FInputActionInstance& ActionInstance)
{
	if(AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		if(ABMMainMenuGameMode* MenuGM = Cast<ABMMainMenuGameMode>(GM))
		{
			MenuGM->AddNewController(this);
		}
	}
}
