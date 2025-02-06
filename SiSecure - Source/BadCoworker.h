// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster.h"
#include "BadCoworker.generated.h"

class UGI_Cybersecurity;
/**
 * 
 */
UCLASS()
class CYBERSECURITE_API UBadCoworker : public UMonster
{
	GENERATED_BODY()
public:
	explicit UBadCoworker(const FObjectInitializer& ObjectInitializer);
	static float GetSpawnTime(UGI_Cybersecurity* gi);
protected:
	inline static float SpawnTime = 180; /*180*/
	virtual void HandleAttack(float DeltaTime) override;
	virtual void HandleShutdown(float DeltaTime) override;
};
