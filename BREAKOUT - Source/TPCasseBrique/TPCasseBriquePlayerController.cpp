// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPCasseBriquePlayerController.h"

#include "CasseBriqueHUD.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TPCasseBriqueCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ATPCasseBriquePlayerController::ATPCasseBriquePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void ATPCasseBriquePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = Cast<UCasseBriqueHUD>(CreateWidget<UUserWidget>(this, HUDWidgetClass));

		if (HUDWidget)
		{
			// Add the widget to the viewport
			HUDWidget->AddToViewport();
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("Widget could not be casted"))
		}
	}

	// Set the input mode to Game Only
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;
}

void ATPCasseBriquePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OnSetDestinationTriggered();
}

void ATPCasseBriquePlayerController::SetupInputComponent()
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
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ATPCasseBriquePlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ATPCasseBriquePlayerController::OnSetDestinationTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ATPCasseBriquePlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ATPCasseBriquePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Completed, this, &ATPCasseBriquePlayerController::OpenPauseMenu);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPCasseBriquePlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void ATPCasseBriquePlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	// FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	/*if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}*/
	FVector TargetDestination = CachedDestination;
	TargetDestination.X = X_Location;
	TargetDestination.Z = 90.0f;
	ControlledPawn->SetActorLocation(TargetDestination);
}

void ATPCasseBriquePlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void ATPCasseBriquePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ATPCasseBriquePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void ATPCasseBriquePlayerController::OpenPauseMenu()
{
	PauseWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
	if(PauseWidgetInstance)
	{
		PauseWidgetInstance->AddToViewport();
	}
}
