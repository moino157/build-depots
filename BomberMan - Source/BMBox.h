// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMIDestroyable.h"
#include "BMIObstacle.h"
#include "GameFramework/Actor.h"
#include "BMBox.generated.h"

class ABMItem;
class ABMMapGrid;
class UNiagaraSystem;

UCLASS()
class BOMBERMAN_API ABMBox : public AActor, public IBMIObstacle, public IBMIDestroyable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABMBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BombHit() override;

	ABMMapGrid* MapRef;

	void ItemRoll();

	void PlayFX();
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* BreakingVFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	USoundBase* BreakingSFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABMItem> RangeItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABMItem> MoreBombsItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABMItem> ShieldItemClass;
};
