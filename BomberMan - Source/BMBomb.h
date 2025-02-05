// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMIDestroyable.h"
#include "BMIObstacle.h"
#include "GameFramework/Actor.h"
#include "BMBomb.generated.h"

class ABomberManCharacter;
class UNiagaraSystem;
class ABMMapGrid;

UCLASS()
class BOMBERMAN_API ABMBomb : public AActor, public IBMIObstacle, public IBMIDestroyable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABMBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BombHit() override;

	ABMMapGrid* MapRef;

	FTimerHandle BombCountdown;

	void OnBombCountdownOver();

	void BombExplosion();

	void PlaySFX();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* FireVFX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FX", meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSFX;

public:
	int Range = 2;

	ABomberManCharacter* Owner;

	void SpawnVFX(FVector location);
};
