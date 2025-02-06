// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "testMansion/HorrorCharacter.h"
#include "testMansion/PuzzleActor.h"


// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	// ...
}


// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPuzzleComponent::SetupPlayerInputAction(UInputMappingContext* mapping_context,
	UInputAction* directional, UInputAction* cancel)
{
	MappingContext = mapping_context;
	IA_DirectionalInput = directional;
	IA_Cancel = cancel;
	SetupPlayerInputAction();
}

void UPuzzleComponent::Directional(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D InputVector = Value.Get<FVector2D>();
	Puzzle->Directional(InputVector);
}

void UPuzzleComponent::Back()
{
	Puzzle->StopPuzzle();
	Deactivate();
	DestroyComponent();
}

void UPuzzleComponent::SetupPlayerInputAction()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		if (EnhancedInputComp)
		{
			EnhancedInputComp->BindAction(IA_DirectionalInput, DirectionalTriggerEvent, this, &UPuzzleComponent::Directional);
			EnhancedInputComp->BindAction(IA_Cancel, ETriggerEvent::Completed, this, &UPuzzleComponent::Back);
		}
	}
}

void UPuzzleComponent::Deactivate()
{
	const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetPlayerController()->GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Log, TEXT("Puzzle component DEACTIVATED. Switching back to character MappingContext"))
		Subsystem->RemoveMappingContext(MappingContext);
		Subsystem->AddMappingContext(Player->GetMappingContext(), 0);

		FInputModeGameOnly InputMode;
		Cast<APlayerController>(Player->GetController())->SetInputMode(InputMode);
		Cast<APlayerController>(Player->GetController())->bShowMouseCursor = false;
	}
}

void UPuzzleComponent::BindToPuzzle(APuzzleActor* NewPuzzle)
{
	if(!NewPuzzle)
		UE_LOG(LogTemp, Error, TEXT("Did not provide actor reference for observable"))
	else
		Puzzle = NewPuzzle;
	
	const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(Player->GetMappingContext());
		Subsystem->AddMappingContext(MappingContext, 1);
	}
	
	Activate(true);
}

void UPuzzleComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	UE_LOG(LogTemp, Warning, TEXT("Observable item component ACTIVATED"))
	const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());
	FInputModeGameAndUI InputMode;
	Player->GetPlayerController()->SetInputMode(InputMode);
	Player->GetPlayerController()->bShowMouseCursor = true;
}

