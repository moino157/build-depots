// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryMenu.h"

#include "The_pacifiers/The_pacifiersCharacter.h"

void UInventoryMenu::InitInventory()
{
	player = Cast<AThe_pacifiersCharacter>(GetOwningPlayer()->GetCharacter());
	SetIsFocusable(true);
}

bool UInventoryMenu::UseItem(AKeyItem* Item)
{
	TArray<FHitResult> HitResults = player->PlayerSweepTrace(player->InteractSphereRadius,player->InteractionTraceDistance,{player});
	for(auto& Hit : HitResults)
	{
		auto* HitActor = Hit.GetActor();
		if(!HitActor) continue;

		if(player->bEnableTraceDebug) DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 50, 32, DebugUtilColor->Red, false, 2, 0, 2);
		if(player->bEnableTraceDebug) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, HitActor->GetName());

		if(!HitActor->Implements<UInteractibleInterface>() || HitActor->IsA(AKeyItem::StaticClass())) continue;
		return IInteractibleInterface::Execute_Interact(HitActor, player, Item);
	}

	if(Item->ItemInformations.bUsableOnNothing)
	{
		player->UseItemOnNothing(Item->ItemInformations.Name);
		return true;
	}

	return false;
}

void UInventoryMenu::RemoveItemFromInventory(FText item_name)
{
	player->RemoveItem(item_name);
}
