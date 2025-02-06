// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractibleInterface.h"
#include "GameFramework/Actor.h"
#include "InteractibleObject.generated.h"

UCLASS()
class THE_PACIFIERS_API AInteractibleObject : public AActor, public IInteractibleInterface
{
	GENERATED_BODY()

public:
	virtual void EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void Focused_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual bool Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item = nullptr) override;
	virtual void Unfocused_Implementation(AThe_pacifiersCharacter* Interactor) override;

	// Sets default values for this actor's properties
	AInteractibleObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
