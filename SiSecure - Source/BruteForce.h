// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BruteForce.generated.h"

/**
 * 
 */
UCLASS()
class CYBERSECURITE_API UBruteForce : public UMonster
{
	GENERATED_BODY()

public:
	explicit UBruteForce(const FObjectInitializer& ObjectInitializer);
	static float GetSpawnTime(UGI_Cybersecurity* gi);
	static float GetSpawnChance(UGI_Cybersecurity* gi);
protected:
	inline static float SpawnTime = FMath::RandRange(30, 60);
	inline static float SpawnChance = 0.3;
	
	virtual void HandleRoaming(float DeltaTime) override;
	virtual void HandleAttack(float DeltaTime) override;
	virtual void HandleDataStealing(float DeltaTime) override;
	virtual void HandleShutdown(float DeltaTime) override;
};
