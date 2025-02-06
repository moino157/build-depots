// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicItem.h"


void APhysicItem::EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::EnteringInteractionRange_Implementation(Interactor);
}

void APhysicItem::ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::ExitingInteractionRange_Implementation(Interactor);
}

void APhysicItem::Focused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Focused_Implementation(Interactor);
}

bool APhysicItem::Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item)
{
	IInteractibleInterface::Interact_Implementation(Interactor, Item);

	player = Interactor;

	if(bIsHeld == false){
		bIsHeld = true;
		player->HeldPhysicItem = this;
		player->bHoldingItem = true;
		MeshComponent->SetEnableGravity(false);
		SetActorTickEnabled(true);
		return true;
	}
	/*else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("false")));
		bIsHeld = false;
		MeshComponent->SetEnableGravity(true);
		SetActorTickEnabled(false);
		return false;
	}*/
	return false;
}

void APhysicItem::Unfocused_Implementation(AThe_pacifiersCharacter* Interactor)
{
	IInteractibleInterface::Unfocused_Implementation(Interactor);
}

void APhysicItem::UpdateObjectPosition()
{
	const FVector hit_location = player->PhysicObjectSweep(1.0f, player->InteractionTraceDistance+player->InteractSphereRadius);
	if(bIsHeld == false)	// Can be set tp false when verifying the sweep in PhysicObjectSweep
	{
		return;
	}
	if(ActorIsStep() == true)
	{
		DropItem();
	}
	else
	{
		FVector new_velocity = (hit_location - GetActorLocation())*16;
		if(bCanBeMoved)
			MeshComponent->SetAllPhysicsLinearVelocity(new_velocity);

		if(bCanBeRotated)
			SetActorRotation(FMath::RInterpTo(
				GetActorRotation()
				,FRotator(0,FRotationMatrix::MakeFromZ(player->GetActorForwardVector()).Rotator().Yaw*YawFactor, 0)
				,0.25f
				,0.25f
			));
		
		MeshComponent->SetAllPhysicsAngularVelocityInRadians(FVector(0,0,0),false);
	}
}

bool APhysicItem::ActorIsStep()
{
	constexpr float max_distance = 75.0f;
	constexpr float shift_minus_z = -100.0f;
	if(((GetActorLocation() - FVector(0,0,shift_minus_z)) - player->GetActorLocation()).Length() <= max_distance)
		return true;
	return false;
}

void APhysicItem::DropItem()
{
	bIsHeld = false;
	player->bHoldingItem = false;
	MeshComponent->SetEnableGravity(true);
	MeshComponent->SetAllPhysicsLinearVelocity((GetVelocity()/1.5).GetClampedToSize(0,777),false);
	SetActorTickEnabled(false);
}

// Sets default values
APhysicItem::APhysicItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("PhysicItem"));
}

// Called when the game starts or when spawned
void APhysicItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bIsHeld)
		UpdateObjectPosition();
}