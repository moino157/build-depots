// Fill out your copyright notice in the Description page of Project Settings.


#include "ObservableActor.h"

#include "InputActionValue.h"
#include "Components/ObservableItemComponent.h"
#include "HorrorCharacter.h"


// Sets default values
AObservableActor::AObservableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AObservableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AObservableActor::ZoomInOut(float movement)
{
	FTransform cam_transform = Player->GetCameraTransform();
	zoom_level = FMath::Clamp((zoom_level+movement), 0, max_zoom);
	SetActorLocation(cam_transform.GetLocation()+(cam_transform.GetRotation().Vector()*(min_zoom+(zoom_level*20))));
}

void AObservableActor::StopObserving()
{
	if(bViewBlend)
		Player->GetPlayerController()->SetViewTargetWithBlend(Player, 0.5f);
	zoom_level = max_zoom;
}

bool AObservableActor::Interact(AHorrorCharacter* InteractingPlayer)
{
	Super::Interact(InteractingPlayer);
	return Observe();
}

bool AObservableActor::Observe()
{
	if(bViewBlend)
		Player->GetPlayerController()->SetViewTargetWithBlend(this, 0.5f);
	Component = NewObject<UObservableItemComponent>(Cast<AActor>(Player), UObservableItemComponent::StaticClass());
	Component->RegisterComponent();
	Component->SetupPlayerInputAction(MappingContext, IA_ToggleRotation, IA_RotateObject, IA_InteractWithObject, IA_ZoomObservable, IA_Back);
	Component->BindToObservable(this);
	return true;
}

void AObservableActor::RotateObject(FRotator InputRotation)
{
	if(RotationEnabled)
	{
		FRotator current_rotation = GetActorRotation();
		SetActorRotation(current_rotation + InputRotation);
	}
}

void AObservableActor::ToggleRotation(bool newToggle)
{
	RotationEnabled = newToggle;
	UE_LOG(LogTemp, Log, TEXT("RotationEnabled: %d"), RotationEnabled)
}

