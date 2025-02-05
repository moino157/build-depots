// Copyright Epic Games, Inc. All Rights Reserved.

#include "BomberManCharacter.h"

#include "BMBomb.h"
#include "BMMapGrid.h"
#include "BomberManPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

ABomberManCharacter::ABomberManCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	Tags.AddUnique("Player");
}

void ABomberManCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ABomberManCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		//AddMovementInput(FVector(1,0,0), MovementVector.Y);
		//AddMovementInput(FVector(0,1,0), MovementVector.X);

		bool moved = false;
		if(MovementVector.Y == 0)
		{
			MapRef->Move(this, {0, static_cast<int>(FMath::CeilToInt(MovementVector.X))});
				
		}
		else
		{
			MapRef->Move(this, {static_cast<int>(FMath::CeilToInt(MovementVector.Y)), 0});
				
		}
		if(moved)
		{
			//Do some animation with sound
		}
	}
}

void ABomberManCharacter::CharacterBombExploded()
{
	HeldBombs = FMath::Clamp(HeldBombs + 1, 0, MaxBombs);
}

void ABomberManCharacter::BeginPlay()
{
	Super::BeginPlay();

	checkf(BombClass, TEXT("Bomb Blueprint not set in character blueprint."));
	
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABMMapGrid::StaticClass(), actors);
	if(actors.Num() > 0)
		MapRef = Cast<ABMMapGrid>(actors[0]);
	if(MapRef == NULL)
		UE_LOG(LogTemp, Error, TEXT("MapRef not set correctly"));

	if(ABomberManPlayerController* controller = Cast<ABomberManPlayerController>(GetController()))
	{
		player_id = controller->GetLocalPlayer()->GetControllerId();
		UE_LOG(LogTemp, Warning, TEXT("Player %d registered"), player_id);
		GetMesh()->SetMaterial(0, CharacterMaterials[player_id]);
		GetMesh()->SetMaterial(1, CharacterMaterials[player_id]);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Could not get player_id: couldnt get controller"))
}

void ABomberManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Setup movement input event
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Started, this, &ABomberManCharacter::Move);
		// Setup bomb button
		EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, this, &ABomberManCharacter::SpawnBomb);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABomberManCharacter::BombHit()
{
	if(bIsShielded)
	{
		bIsShielded = false;
		UGameplayStatics::PlaySound2D(GetWorld(),PowerupSounds[3]);
		GetMesh()->SetMaterial(1, CharacterMaterials[player_id]);
	}
	else
	{
		MapRef->RemovePawnFromGrid(this);
		MapRef->OnPlayerDie(this);
		Destroy();
	}
}

void ABomberManCharacter::SpawnBomb()
{
	if(HeldBombs <= 0)
		return;
	FVector spawn_loc = GetActorLocation() + FVector(-50,-50,-96);
	if(MapRef->CanGoOnTile(spawn_loc, this))
	{
		ABMBomb* Bomb = GetWorld()->SpawnActor<ABMBomb>(BombClass, spawn_loc, FRotator(0,0,0));
		MapRef->AddNewPawnToGrid(Bomb);
		HeldBombs = FMath::Clamp(HeldBombs - 1, 0, MaxBombs);
		Bomb->Owner = this;
		Bomb->Range = BombRange;
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't spawn bomb at location %s"), *spawn_loc.ToString());
	}
}

void ABomberManCharacter::UpgradeBombNumber()
{
	MaxBombs = MaxBombs + 1;
	HeldBombs = FMath::Clamp(HeldBombs + 1,0, MaxBombs);
	UGameplayStatics::PlaySound2D(GetWorld(),PowerupSounds[0]);
	UE_LOG(LogTemp, Warning, TEXT("Added more bombs to player. MaxBombs is now %d"), MaxBombs);
}

void ABomberManCharacter::UpgradeBombRange()
{
	BombRange = BombRange + 1;
	UGameplayStatics::PlaySound2D(GetWorld(),PowerupSounds[1]);
	UE_LOG(LogTemp, Warning, TEXT("Added range. Range is now %d"), BombRange);
}

void ABomberManCharacter::UpgradeShield()
{
	bIsShielded = true;
	GetMesh()->SetMaterial(1, CharacterMaterials.Last());
	UGameplayStatics::PlaySound2D(GetWorld(),PowerupSounds[2]);
	UE_LOG(LogTemp, Warning, TEXT("Added shield. Shield is now %d"), bIsShielded);
}
