// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractibleInterface.h"
#include "GameFramework/Actor.h"
#include "LockedObject.generated.h"

UCLASS()
class THE_PACIFIERS_API ALockedObject : public AActor, public IInteractibleInterface
{
public:
	virtual void EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void Focused_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual bool Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item = nullptr) override;
	virtual void Unfocused_Implementation(AThe_pacifiersCharacter* Interactor) override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALockedObject();

	// /** Required item to unlock this object */
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Item)
	// FText RequiredKeyItemName = FText::FromString("!NO NAME!");

	/** Array  of Required items to unlock this object */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Item)
	TArray<FText> RequiredKeyItemNames;
	
	/** If the player used the key item required to unlock this object */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Item)
	bool bIsUnlocked = false;
	
	/** If the player had to manually select the required key item in his inventory */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Item)
	bool bAutoUseKey = true;

	/** Triggers when the player interacts with this actor and has the required key item */
	UFUNCTION(BlueprintNativeEvent, Category = Item)
	void Unlock(AThe_pacifiersCharacter* Interactor, const FText& RequiredKeyItemName);

	/** Triggers when the player interacts with this actor but does not possess the required key item */
	UFUNCTION(BlueprintNativeEvent, Category = Item)
	void Locked(AThe_pacifiersCharacter* Interactor);

	/** Triggers when the player interacts with the unlocked object */
	UFUNCTION(BlueprintNativeEvent, Category = Item)
	void Open();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
