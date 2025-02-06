// Copyright Epic Games, Inc. All Rights Reserved.

#include "The_pacifiersCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PhysicItem.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#define DebugPrint(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AThe_pacifiersCharacter

AThe_pacifiersCharacter::AThe_pacifiersCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 115.0f);

	QuestComponent = CreateDefaultSubobject<UTpQuestComponent>(TEXT("Quest Component"));

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	// Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	// Mesh1P->SetOnlyOwnerSee(true);
	// Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	// Mesh1P->bCastDynamicShadow = false;
	// Mesh1P->CastShadow = false;
	// Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	// Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AThe_pacifiersCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AThe_pacifiersCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThe_pacifiersCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AThe_pacifiersCharacter::Move_Start);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AThe_pacifiersCharacter::Move_Stop);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThe_pacifiersCharacter::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThe_pacifiersCharacter::OnInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AThe_pacifiersCharacter::OnInteractReleased);

		// Flashlight
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Triggered, this, &AThe_pacifiersCharacter::ToggleFlashlight);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AThe_pacifiersCharacter::ToggleCrouch);
		
		// Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AThe_pacifiersCharacter::ToggleInventory);

		// Pause
		//EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AThe_pacifiersCharacter::ToggleFlashlight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThe_pacifiersCharacter::Sprint_Start()
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	
	if(bGodMode)
	{
		MovementComponent->MaxWalkSpeed = run_speed;
	}else
	{
		if(stamina_current > 0)
		{
			if(bIsCrouching)
			{
				OnCrouchStopCalled(true);
			}else
			{
				bIsRunning = true;
				MovementComponent->MaxWalkSpeed = run_speed;
			}
		}
	}
}

bool AThe_pacifiersCharacter::StaminaRemove()
{
	if(stamina_current == 0 || bIsRunning == false)
	{
		Sprint_Stop();
		return false;
	}else
	{
		if(GetVelocity().Length() > walk_speed)
		{
			stamina_current = FMath::Clamp(stamina_current-1, 0.0f, stamina_max);
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("%f"), stamina_current));
		}
		return true;
	}
}

void AThe_pacifiersCharacter::Sprint_Stop()
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	if(bIsCrouching)
	{
		MovementComponent->MaxWalkSpeed = crouch_speed;
	}else
	{
		MovementComponent->MaxWalkSpeed = walk_speed;
	}
	bIsRunning = false;
}

bool AThe_pacifiersCharacter::StaminaAdd()
{
	if(bIsRunning == false || GetVelocity().Length() <= walk_speed)
	{
		stamina_current = FMath::Clamp(stamina_current+1, 0.0f, stamina_max);
	}
	return true;
}

void AThe_pacifiersCharacter::StaminaSet(float NewStamina)
{
	stamina_current = FMath::Clamp(NewStamina, 0.0f, stamina_max);
}

void AThe_pacifiersCharacter::ToggleCrouch()
{
	if(bIsCrouching)
	{
		OnCrouchStopCalled(false);
	}else
	{
		Crouch_Start();
	}
}

void AThe_pacifiersCharacter::Crouch_Start()
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	if(MovementComponent->CanCrouchInCurrentState() == false)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Cannot crouch")));
		return;
	}
	
	if(bIsRunning)
	{
		Sprint_Stop();
	}

	MovementComponent->MaxWalkSpeed = crouch_speed;
	TL_Crouch->Play();
	bIsCrouching = true;
}

void AThe_pacifiersCharacter::Crouch_Stop(bool bIsTryingToRun)
{
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	MovementComponent->MaxWalkSpeed = walk_speed;
	TL_Crouch->Reverse();
	bIsCrouching = false;

	if(bIsTryingToRun)
	{
		Sprint_Start();
	}
}

void AThe_pacifiersCharacter::OnCrouchStopCalled_Implementation(bool bIsTryingToRun)
{
	// Logic needed when blueprints don't implement the event. Can be empty.
}

void AThe_pacifiersCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);

		// handle footstep sound
		if(GetVelocity().Length() > 0)
		{
			/*if(GetVelocity().Length() > walk_speed)
			{
				TL_Footstep->SetPlayRate()
			}else if(GetVelocity().Length() > crouch_speed)
			{
				
			}else
			{
				
			}*/
			TL_Footstep->SetPlayRate(GetVelocity().Length()/walk_speed);
			TL_Footstep->Play();
		}else
		{
			TL_Footstep->Stop();
		}
	}
}

void AThe_pacifiersCharacter::Move_Start()
{
}

void AThe_pacifiersCharacter::Move_Stop()
{
	TL_Footstep->Stop();
}

void AThe_pacifiersCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThe_pacifiersCharacter::AddItem(AKeyItem* item)
{
	if(item->bIsNote)
		Notes.Add(item);
	else
		Items.Add(item);
}

void AThe_pacifiersCharacter::RemoveItem(FText item_name)
{
	for(AKeyItem* item: Items)
	{
		if(item->ItemInformations.Name.EqualTo(item_name))
		{
			Items.Remove(item);
			return;
		}
	}
}

void AThe_pacifiersCharacter::DropHeldItem()
{
	HeldPhysicItem->DropItem();
}

AKeyItem* AThe_pacifiersCharacter::HasItem(FText item_name)
{
	for(AKeyItem* item: Items)
	{
		if(item->ItemInformations.Name.EqualTo(item_name)) return item;
	}

	return nullptr;
}

void AThe_pacifiersCharacter::UseItem(FText item_name)
{
	for(AKeyItem* Item: Items)
	{
		if(Item->ItemInformations.Name.EqualTo(item_name))
		{
			if(Item->ItemInformations.bConsumeOnUse == true)
			{
				Item->ItemInformations.Quantity--;
				if(Item->ItemInformations.Quantity <= 0)
				{
					Items.Remove(Item);
					if(Item->bShowDiscardMessage)
						DiscardItemMessage(item_name);
				}
			}
			return;
		}
	}
}

void AThe_pacifiersCharacter::UseItemOnNothing_Implementation(const FText& Item_Name)
{
	UseItem(Item_Name);
}

void AThe_pacifiersCharacter::DiscardItemMessage_Implementation(const FText& Item_Name)
{
}

void AThe_pacifiersCharacter::CameraRotation()
{
	FRotator cam_rot = FirstPersonCameraComponent->GetComponentRotation();
	FRotator player_rot = GetCapsuleComponent()->GetComponentRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(cam_rot, player_rot);
	float angle = UKismetMathLibrary::Abs(delta.Yaw);
	if(angle >= 100)
	{
		float new_lag_speed = ((angle - 100)/50*100) + 7;
		FirstPersonSpringArmComponent->CameraRotationLagSpeed = new_lag_speed;
	}else
	{
		FirstPersonSpringArmComponent->CameraRotationLagSpeed = 7;
	}
}

void AThe_pacifiersCharacter::CameraShake()
{	// If player controller exists
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		const float velocity = GetVelocity().Length();
		if(velocity >= 1 && CanJump())
		{
			if(velocity < run_speed)
			{
				PlayerController->ClientStartCameraShake(CameraShakes[1]);
			}else
			{
				PlayerController->ClientStartCameraShake(CameraShakes[2]);
			}
		}else
		{
			PlayerController->ClientStartCameraShake(CameraShakes[0]);
		}
	}
	
}

void AThe_pacifiersCharacter::OnInteract()
{
	Interact();
}

bool AThe_pacifiersCharacter::IsValidHit(FVector HitLocation)
{
	const auto EyeLocation = GetFirstPersonCameraComponent()->GetComponentLocation();

	// Used to verify if the player is interacting through wall
	FHitResult Confirmation;
	const bool bHitFound = GetWorld()->LineTraceSingleByChannel(Confirmation, EyeLocation, HitLocation
		, ECollisionChannel::ECC_Visibility);
	
	
	if(bHitFound == true)
	{
		if(Confirmation.GetActor()->Implements<UInteractibleInterface>())
			return true;
		//DebugPrint(TEXT("Hit invalidated by"));
		//DebugPrint(Confirmation.GetActor()->GetName());
		return false;
	}
	
	return true;
}

FVector AThe_pacifiersCharacter::Interact()
{
	const auto EyeLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	const auto EyeRotation = GetFirstPersonCameraComponent()->GetComponentRotation();

	const FVector End = EyeLocation + (EyeRotation.Vector() * InteractionTraceDistance);

	TArray<FHitResult> InteractibleHitResults;
	TArray<FHitResult> KeyItemHitResults;
	TArray<FHitResult> LockedObjectResult;
	TArray<FHitResult> PhysicObjectResult;
	FCollisionShape Shape;
	Shape.SetSphere(InteractSphereRadius);

	bool bPhysicItemHitFound = GetWorld()->SweepMultiByChannel(PhysicObjectResult, EyeLocation, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel5, Shape);
	bool bItemHitFound = GetWorld()->SweepMultiByChannel(KeyItemHitResults, EyeLocation, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel3, Shape);
	bool bLockedItemHitFound = GetWorld()->SweepMultiByChannel(LockedObjectResult, EyeLocation, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, Shape);
	bool bInteractibleHitFound = GetWorld()->SweepMultiByChannel(InteractibleHitResults, EyeLocation, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, Shape);
	
	if(bEnableTraceDebug) DrawDebugLine(GetWorld(), EyeLocation, End, DebugUtilColor->Green, false, 2.0f, 0, 1.0f);

	// If no hit detected, quit function
	if(bInteractibleHitFound == false) return End;

	// Check for key items first since they are the priority
	if(bItemHitFound)
	{
		FVector KeyEnd = KeyItemHitResults[0].ImpactPoint;
		FHitResult ValidationHit1;	// Locked Item
		FHitResult ValidationHit2;	// Physic Item
		bool bLockedItemBetween = GetWorld()->LineTraceSingleByChannel(ValidationHit1, EyeLocation, KeyEnd, ECollisionChannel::ECC_GameTraceChannel2);
		bool bPhysicItemBetween = GetWorld()->LineTraceSingleByChannel(ValidationHit2, EyeLocation, KeyEnd, ECollisionChannel::ECC_GameTraceChannel5);
		if(bLockedItemBetween == false && bPhysicItemBetween == false)
		{
			if(bEnableTraceDebug) DrawDebugSphere(GetWorld(), ValidationHit1.ImpactPoint, InteractSphereRadius, 32, DebugUtilColor->Red, false, 1, 0, 1);
			if(bEnableTraceDebug) DebugPrint(KeyItemHitResults[0].GetActor()->GetName());
			IInteractibleInterface::Execute_Interact(KeyItemHitResults[0].GetActor(), this, nullptr);
			return KeyEnd;
		}
	}
	
	// Check each hit object
	for(auto& Hit : InteractibleHitResults)
	{
		auto* HitActor = Hit.GetActor();
		if(!HitActor) continue;

		if(!HitActor->Implements<UInteractibleInterface>()) continue;
		if(bEnableTraceDebug) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, InteractSphereRadius, 32, DebugUtilColor->Red, false, 1, 0, 1);
		if(bEnableTraceDebug) DebugPrint(HitActor->GetName());
		if(!IsValidHit(Hit.ImpactPoint)) continue;

		IInteractibleInterface::Execute_Interact(HitActor, this, nullptr);
		return Hit.ImpactPoint;
	}
	
	return End;
}

void AThe_pacifiersCharacter::OnInteractReleased()
{
	if(HeldPhysicItem != nullptr)
	{
		DropHeldItem();
	}
}

TArray<FHitResult> AThe_pacifiersCharacter::PlayerSweepTrace(float sphere_radius, float distance, TArray<AActor*> ToIgnore, bool InteractibleOnly) const
{
	const auto EyeLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	const auto EyeRotation = GetFirstPersonCameraComponent()->GetComponentRotation();

	const FVector End = EyeLocation + (EyeRotation.Vector() * distance);

	TArray<FHitResult> HitResults;
	FCollisionShape Shape;
	Shape.SetSphere(sphere_radius);

	if(InteractibleOnly == true)
	{
		// Used for interactible
		bool bHitFound = GetWorld()->SweepMultiByChannel(HitResults, EyeLocation, End, FQuat::Identity
			, ECollisionChannel::ECC_GameTraceChannel4, Shape);
	}else
	{	// Used for moving physics items around
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActors(ToIgnore);
		bool bHitFound = GetWorld()->SweepMultiByChannel(HitResults, EyeLocation, End, FQuat::Identity
			, ECollisionChannel::ECC_Visibility, Shape, TraceParams);
	}
	
	if(bEnableTraceDebug) DrawDebugLine(GetWorld(), EyeLocation, End, DebugUtilColor->Green, false, 2.0f, 0, 1.0f);

	return HitResults;
}

FVector AThe_pacifiersCharacter::PhysicObjectSweep(float sphere_radius, float distance)
{
	const auto EyeLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
	const auto EyeRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
	const FVector End = EyeLocation + (EyeRotation.Vector() * distance);
	
	TArray<FHitResult> HitResults = PlayerSweepTrace(sphere_radius,distance, {HeldPhysicItem}, false);
	TArray<FHitResult> HitResultsVerification = PlayerSweepTrace(InteractSphereRadius,distance, {HeldPhysicItem}, true);
	// If object is too far, drop it
	if(HitResultsVerification.Num() == 0)
	{
		//DebugPrint(TEXT("HitResultsVerification.Num() == 0"));
		DropHeldItem();
		return FVector(0,0,0);
	}
	if(HitResults.Num() > 0)
	{
		// If there is a wall between the interactible and the player, Drop the item
		float PhysicObjectDistance = (HitResults[0].ImpactPoint - EyeLocation).Length();
		float InteractibleObjectDistance = (HitResultsVerification[0].ImpactPoint - EyeLocation).Length();
		if(PhysicObjectDistance < InteractibleObjectDistance)
		{
			//DebugPrint(TEXT("PhysicObjectDistance < InteractibleObjectDistance"));
			if(bEnableTraceDebug) DrawDebugSphere(GetWorld(), HitResults[0].ImpactPoint, InteractSphereRadius, 32, DebugUtilColor->Red, false, 5, 0, 1);
			if(bEnableTraceDebug) DrawDebugSphere(GetWorld(), HitResultsVerification[0].ImpactPoint, InteractSphereRadius, 32, DebugUtilColor->Green, false, 5, 0, 1);

			if(bEnableTraceDebug)DebugPrint(FString::Printf(TEXT("%f"), PhysicObjectDistance));
			if(bEnableTraceDebug)DebugPrint(HitResults[0].GetActor()->GetName());

			if(bEnableTraceDebug)DebugPrint(FString::Printf(TEXT("%f"), InteractibleObjectDistance));
			if(bEnableTraceDebug)DebugPrint(HitResultsVerification[0].GetActor()->GetName());
			
			DropHeldItem();
			return FVector(0,0,0);
		}else
		{
			return HitResults[0].Location;
		}
	}
	
	return End;
}

void AThe_pacifiersCharacter::ToggleFlashlight()
{
	if(HasItem(FText::FromString("Flashlight")))
	{
		if(bHasBattery == false)
		{
			OnNoBatteryTurnOn();
			return;
		}
		if(bIsFlashlightOn)
		{
			bIsFlashlightOn = false;
			SL_Flashlight->SetIntensity(0.0);
			OnFlashLightTurnOff();
		}else
		{
			bIsFlashlightOn = true;
			SL_Flashlight->SetIntensity(40000.0);
			OnFlashLightTurnOn();
		}
	}
}

void AThe_pacifiersCharacter::ToggleInventory()
{
	OnInventoryAction();
	
	if(bEnableTraceDebug == false) return;;
	
	for(AKeyItem* item : Items)
	{
		if(GEngine)
		{
			DebugPrint(item->ItemInformations.Description.ToString());
			DebugPrint(item->ItemInformations.Name.ToString());
		}
	}
	
	if(GEngine)
		DebugPrint(FString::Printf(TEXT("Inventory: %d"), Items.Num()));
}

void AThe_pacifiersCharacter::OnFlashLightTurnOn_Implementation()
{
	// Logic needed when blueprints don't implement the event. Can be empty.
}

void AThe_pacifiersCharacter::OnFlashLightTurnOff_Implementation()
{
	// Logic needed when blueprints don't implement the event. Can be empty.
}

void AThe_pacifiersCharacter::OnNoBatteryTurnOn_Implementation()
{
}