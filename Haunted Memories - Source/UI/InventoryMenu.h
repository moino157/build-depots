// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "The_pacifiers/KeyItem.h"
#include "InventoryMenu.generated.h"

class AThe_pacifiersCharacter;
/**
 * 
 */
UCLASS()
class THE_PACIFIERS_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
protected:

	UFUNCTION(BlueprintCallable, Category = Init)
	void InitInventory();

	/** Array of items that were added to the inventory menu */
	UPROPERTY(BlueprintReadWrite, Category = Item)
	TArray<AKeyItem*> ShownItems;

	/** Array of notes that were added to the inventory menu */
	UPROPERTY(BlueprintReadWrite, Category = Item)
	TArray<AKeyItem*> ShownNotes;

	UPROPERTY(BlueprintReadOnly)
	AThe_pacifiersCharacter* player;

	/** Called when the object is manually used. Interacts with object if front of player. */
	UFUNCTION(BlueprintCallable, Category = Interact)
	bool UseItem(AKeyItem* Item);

	/** Called when an item is removed from the player's inventory */
	UFUNCTION(BlueprintCallable, Category = Item)
	void RemoveItemFromInventory(FText item_name);
	
public:
	
	/** Array of items held by the player passed in parameter when creating the widget */
	UPROPERTY(BlueprintReadWrite, Category = Item)
	TArray<AKeyItem*> HeldItems;

	/** Array of notes collected by the player passed in parameter when creating the widget */
	UPROPERTY(BlueprintReadWrite, Category = Item)
	TArray<AKeyItem*> HeldNotes;
};
