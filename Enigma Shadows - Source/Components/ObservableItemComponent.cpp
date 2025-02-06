// Fill out your copyright notice in the Description page of Project Settings.


#include "ObservableItemComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "testMansion/HorrorCharacter.h"
#include "testMansion/ObservableActor.h"


// Sets default values for this component's properties
UObservableItemComponent::UObservableItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	// ...
}


// Called when the game starts
void UObservableItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UObservableItemComponent::SetupPlayerInputAction(UInputMappingContext* mapping_context,
	UInputAction* toggle_rotation, UInputAction* rotate_object, UInputAction* interact_with_object, UInputAction* zoom,
	UInputAction* back)
{
	MappingContext = mapping_context;
	IA_ToggleRotation = toggle_rotation;
	IA_RotateObject = rotate_object;
	IA_InteractWithObject = interact_with_object;
	IA_ZoomObservable = zoom;
	IA_Back = back;
	SetupPlayerInputAction();
}


void UObservableItemComponent::ZoomInOut(const FInputActionValue& Value)
{
	if(CurrentObservable->bCanBeMoved)
	{
		CurrentObservable->ZoomInOut(FMath::Clamp(Value.Get<float>(), -1, 1));
	}
}

void UObservableItemComponent::Back()
{
	CurrentObservable->StopObserving();
	Deactivate();
	DestroyComponent();
}

// Called every frame
void UObservableItemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UObservableItemComponent::SetupPlayerInputAction()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		if (EnhancedInputComp)
		{
			EnhancedInputComp->BindAction(IA_RotateObject, ETriggerEvent::Triggered, this, &UObservableItemComponent::RotateObject);
			EnhancedInputComp->BindAction(IA_ToggleRotation, ETriggerEvent::Started, this, &UObservableItemComponent::ToggleRotation);
			EnhancedInputComp->BindAction(IA_ToggleRotation, ETriggerEvent::Completed, this, &UObservableItemComponent::ToggleRotation);
			EnhancedInputComp->BindAction(IA_ZoomObservable, ETriggerEvent::Triggered, this, &UObservableItemComponent::ZoomInOut);
			EnhancedInputComp->BindAction(IA_Back, ETriggerEvent::Completed, this, &UObservableItemComponent::Back);
			EnhancedInputComp->BindAction(IA_InteractWithObject, ETriggerEvent::Completed, this, &UObservableItemComponent::InteractWithObject);
		}
	}
}

void UObservableItemComponent::Deactivate()
{
	const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetPlayerController()->GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Log, TEXT("Observable item component DEACTIVATED. Switching back to character MappingContext"))
		Subsystem->RemoveMappingContext(MappingContext);
		Subsystem->AddMappingContext(Player->GetMappingContext(), 0);

		FInputModeGameOnly InputMode;
		Cast<APlayerController>(Player->GetController())->SetInputMode(InputMode);
		Cast<APlayerController>(Player->GetController())->bShowMouseCursor = false;
	}
}

void UObservableItemComponent::ToggleRotation(const FInputActionValue& Value)
{
	if(CurrentObservable->bCanBeRotated)
	{
		const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());
		Player->GetPlayerController()->bShowMouseCursor = !Value.Get<bool>();
		CurrentObservable->ToggleRotation(Value.Get<bool>());
	}
}

void UObservableItemComponent::RotateObject(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FRotator input_rotation = FRotator(MovementVector.Y, MovementVector.X, 0);
	CurrentObservable->RotateObject(input_rotation);
}

void UObservableItemComponent::BindToObservable(AObservableActor* NewObservable)
{
	if(!NewObservable)
		UE_LOG(LogTemp, Error, TEXT("Did not provide actor reference for observable"))
	else
		CurrentObservable = NewObservable;
	
	const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Player->GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(Player->GetMappingContext());
		Subsystem->AddMappingContext(MappingContext, 1);
	}
	
	Activate(true);
}

void UObservableItemComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	UE_LOG(LogTemp, Warning, TEXT("Observable item component ACTIVATED"))
	const AHorrorCharacter* Player = Cast<AHorrorCharacter>(GetOwner());
	FInputModeGameAndUI InputMode;
	Player->GetPlayerController()->SetInputMode(InputMode);
	Player->GetPlayerController()->bShowMouseCursor = true;
}

void UObservableItemComponent::InteractWithObject(const FInputActionValue& Value)
{
	CurrentObservable->InteractWithObject();
}

