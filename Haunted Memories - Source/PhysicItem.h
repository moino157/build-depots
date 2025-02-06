// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractibleInterface.h"
#include "The_pacifiersCharacter.h"
#include "GameFramework/Actor.h"
#include "PhysicItem.generated.h"

UCLASS()
class THE_PACIFIERS_API APhysicItem : public AActor, public IInteractibleInterface
{
public:
	virtual void EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void Focused_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual bool Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item = nullptr) override;
	virtual void Unfocused_Implementation(AThe_pacifiersCharacter* Interactor) override;

private:
	GENERATED_BODY()

	AThe_pacifiersCharacter* player;
	bool bIsHeld = false;

	/** Update position of object when held by player; called in tick */
	void UpdateObjectPosition();
	/** Verify if player is stepping on physic object */
	bool ActorIsStep();

public:
	// Sets default values for this actor's properties
	APhysicItem();
	
	/** Called when the object is dropped by player; sets gravity */
	void DropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = Mesh)
	UMeshComponent* MeshComponent;

	/** Object's location will not be affected; no translation will be applied by the player */
	UPROPERTY(EditAnywhere, Category = Physic)
	bool bCanBeMoved = true;

	/** Object's rotation will not be affected; no rotation will be applied by the player */
	UPROPERTY(EditAnywhere, Category = Physic)
	bool bCanBeRotated = true;

	/** A scaling factor for rotating the object in yaw */
	UPROPERTY(EditAnywhere, Category = Physic)
	float YawFactor = 1.0f;

	/** An offset for holding the object; used to adjust the holding location in front of the player */
	//UPROPERTY(EditAnywhere, Category = Physic)
	//FVector LocationOffset = FVector(0,0,0);

	///** If true, the offset is calculated when the player picks up the object */
	//UPROPERTY(EditAnywhere, Category = Physic)
	//bool bDynamicOffset = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
