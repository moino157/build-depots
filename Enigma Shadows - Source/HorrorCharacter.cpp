// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHorrorCharacter::AHorrorCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get the local player subsystem
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	TArray<UCameraComponent*> CameraComps;
	GetComponents<UCameraComponent>(CameraComps);
	CameraComponent = CameraComps.Last();
}

void AHorrorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHorrorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
	}
}

void AHorrorCharacter::Sprint(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	UE_LOG(LogTemp, Warning, TEXT("Sprint: %d"), isPressed)
	if(isPressed)
		GetCharacterMovement()->MaxWalkSpeed = running_speed;
	else
		GetCharacterMovement()->MaxWalkSpeed = walking_speed;
}

// Called every frame
void AHorrorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform AHorrorCharacter::GetCameraTransform()
{
	FVector loc = CameraComponent->GetComponentLocation();
	FRotator rot = CameraComponent->GetComponentRotation();
	return FTransform(rot, loc);
}

APlayerController* AHorrorCharacter::GetPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

// Called to bind functionality to input
void AHorrorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AHorrorCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AHorrorCharacter::Look);
		// Sprinting
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &AHorrorCharacter::Sprint);
		EnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &AHorrorCharacter::Sprint);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

