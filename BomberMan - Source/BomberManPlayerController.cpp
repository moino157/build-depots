// Copyright Epic Games, Inc. All Rights Reserved.

#include "BomberManPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "BomberManCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ABomberManPlayerController::ABomberManPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void ABomberManPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	FInputModeGameOnly InputMode;
	APlayerController::SetInputMode(InputMode);
	bShowMouseCursor = false;
}

void ABomberManPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ABomberManPlayerController::OnInputStarted);
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ABomberManPlayerController::OnSetDestinationTriggered);
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ABomberManPlayerController::OnSetDestinationReleased);
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ABomberManPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ABomberManPlayerController::OnInputStarted);
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ABomberManPlayerController::OnTouchTriggered);
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ABomberManPlayerController::OnTouchReleased);
		// EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ABomberManPlayerController::OnTouchReleased)
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABomberManPlayerController::OnInputStarted()
{
	
}

// Triggered every frame when the input is held down
void ABomberManPlayerController::OnSetDestinationTriggered()
{
	
}

void ABomberManPlayerController::OnSetDestinationReleased()
{
	
}

// Triggered every frame when the input is held down
void ABomberManPlayerController::OnTouchTriggered()
{
	// OnSetDestinationTriggered();
}

void ABomberManPlayerController::OnTouchReleased()
{
}