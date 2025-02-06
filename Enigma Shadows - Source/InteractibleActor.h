// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractibleInterface.h"
#include "Interfaces/ObservableInterface.h"
#include "InteractibleActor.generated.h"

class AHorrorCharacter;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class TESTMANSION_API AInteractibleActor : public AActor, public IInteractibleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractibleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	AHorrorCharacter* Player;

	bool RotationEnabled = false;

	UFUNCTION(BlueprintCallable)
	virtual bool Interact(AHorrorCharacter* InteractingPlayer) override;


public:

};
