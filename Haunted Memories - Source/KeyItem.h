// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractibleInterface.h"
#include "GameFramework/Actor.h"
#include "KeyItem.generated.h"

USTRUCT(BlueprintType, DisplayName = "Item Informations", Category = Item)
struct FItemStruct
{
    GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Item)
	FText Name = FText::FromString("!NO NAME!");
	UPROPERTY(BlueprintReadWrite, Category = Item)
	FText Description = FText::FromString("!NO DESCRIPTION!");
	UPROPERTY(BlueprintReadWrite, Category = Item)
	FText Content = FText::FromString("!NO CONTENT!");
	UPROPERTY(BlueprintReadWrite, Category = Item)
	UTexture2D* Icon;
	UPROPERTY(BlueprintReadWrite, Category = Item)
	UStaticMesh* Mesh;
	UPROPERTY(BlueprintReadWrite, Category = Item)
	int Quantity = 1;
	UPROPERTY(BlueprintReadWrite, Category = Item)
	bool bConsumeOnUse = false;
	UPROPERTY(BlueprintReadWrite, Category = Item)
	bool bUsableOnNothing = false;
};

UCLASS()
class THE_PACIFIERS_API AKeyItem : public AActor, public IInteractibleInterface
{
	GENERATED_BODY()

public:
	virtual void EnteringInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void ExitingInteractionRange_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual void Focused_Implementation(AThe_pacifiersCharacter* Interactor) override;
	virtual bool Interact_Implementation(AThe_pacifiersCharacter* Interactor, AKeyItem* Item = nullptr) override;
	virtual void Unfocused_Implementation(AThe_pacifiersCharacter* Interactor) override;
	
	// Sets default values for this actor's properties
	AKeyItem();

	/** Calls blueprint function when object is interacted with. */
	UFUNCTION(BlueprintNativeEvent, Category = Item)
	void Pickup();
	
	UPROPERTY(BlueprintReadWrite, Category = Item)
	FItemStruct ItemInformations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	bool bIsNote = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	bool bHasBeenPickedUp = false;

	UPROPERTY(BlueprintReadWrite, Category = Item)
	bool bShowDiscardMessage = true;
	
	UPROPERTY(BlueprintReadWrite, Category = Item)
	bool bShowOnPickup = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	bool bCanBePickedUp = true;

	UFUNCTION(BlueprintNativeEvent, Category = Item)
	void CannotBePickedUp();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};