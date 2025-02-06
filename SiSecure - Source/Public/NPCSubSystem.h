// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Cybersecurite/Monster.h"
#include "NPCSubSystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterSpawn, UMonster*, MonsterUObject);

UCLASS()
class CYBERSECURITE_API UNPCSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;

	// spawn a monster simulator (with a state machine starting at roaming)
	UFUNCTION(BlueprintCallable, Category = "Monsters")
	UMonster* SpawnMonsterNoTarget(EMonsterType type);
	UFUNCTION(BlueprintCallable, Category = "Monsters")
	UMonster* SpawnMonster(EMonsterType type, ECampus target);
	UFUNCTION(BlueprintCallable, Category = "Monsters")
	UMonster* SpawnMonsterWithState(EMonsterType type, ECampus target, EMonsterState state);

	// Used with TimerHandle
	UFUNCTION(Category = "Monsters")
	void SpawnMonsterDelayed(EMonsterType type);
	
	UFUNCTION(Category = "Monsters")
	void HandleMonsterStateChange(EMonsterState PreviousState, EMonsterState CurrentState, UMonster* Object);

	UFUNCTION(Category = "Monsters")
	ECampus GetCoworkerTargetCampus();

	// RNG to spawn a monster of specified type
	UFUNCTION(BlueprintCallable, Category = "Monsters")
	bool RollMonsterSpawn(ECampus PlayerLocation, EMonsterType type);

	int GetMonsterTypeCount(EMonsterType type);

	// remove a monster from the list and mark it as garbage (does not destroy the physical copy only the simulation)
	UFUNCTION(BlueprintCallable, Category = "Monsters")
	void DestroyMonster(UMonster* Monster);

	// return a copy of all the instantiated monsters
	UFUNCTION(BlueprintCallable, Category = "Monsters")
	const TArray<UMonster*>& GetMonsters() const;

	UFUNCTION(BlueprintCallable, Category = "Monsters", BlueprintPure)
	bool HasFirstMonsterSpawned();

	// Called when a monster is spawned
	UPROPERTY(BlueprintAssignable)
	FOnMonsterSpawn OnMonsterSpawnDelegate;

	void UpdateMonsters();

	//helper function
	bool IsTargetingState(EMonsterState State);

	UFUNCTION()
	void ResetSystem();

	UFUNCTION()
	void StartSystem();
	
private:
	UPROPERTY()
	TArray<UMonster*> Monsters;

	// Timer handle for the monster updates
	FTimerHandle MonsterUpdateTimerHandle;
	// Timer that handles spawning a BruteForce after one that disappears
	FTimerHandle BruteForceSpawnTimer;
	// Timer that handles spawning a bomb carrying coworker
	FTimerHandle BadCoworkerSpawnTimer;

	// Update interval (every 0.1 seconds / 60 fps)
	float UpdateInterval = 0.1f;

	// Flag indicating if the first monster spawned at launch
	bool bFirstMonsterSpawned = false;


};
