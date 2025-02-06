// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Monster.generated.h"

/**
 * 
 */

class UGI_Cybersecurity;

UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	BruteForce,
	CoWorker
};

enum class ECampus : uint8;
UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Roaming      UMETA(DisplayName = "Roaming"),
	Traveling      UMETA(DisplayName = "Traveling"),
	Attack       UMETA(DisplayName = "Attack"),
	DataStealing UMETA(DisplayName = "DataStealing"),
	Shutdown     UMETA(DisplayName = "Shutdown"),
	Death        UMETA(DisplayName = "Death")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMonsterDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoTargetFound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterStateChanged, EMonsterState, State);

UCLASS()
class CYBERSECURITE_API UMonster : public UObject
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMonsterAttackStateChanged, EMonsterState, PreviousState, EMonsterState, CurrentState, UMonster*, Object);
	UPROPERTY(BlueprintAssignable)
	FOnMonsterAttackStateChanged OnMonsterAttackStateChangedDelegate;
	
	// The current state of the monster
	UPROPERTY(BlueprintReadWrite, Category="State")
	EMonsterState CurrentState;
	
	UPROPERTY(BlueprintReadWrite, Category="State")
	EMonsterState PreviousState;

	UPROPERTY(BlueprintReadWrite, Category="State")
	float CurrentStateTime;

	UPROPERTY(BlueprintReadWrite, Category="target")
	ECampus Target;

	UPROPERTY(BlueprintReadWrite, Category="type")
	EMonsterType Type;

	UPROPERTY(BlueprintReadWrite, Category = "Visuals")
	FTransform StartTranform;

	UPROPERTY(BlueprintReadWrite, Category = "Visuals")
	FTransform TargetTranform;

	UFUNCTION(BlueprintCallable, Category="State")
	void Update(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category="State")
	void SetState(EMonsterState NewState);
	void SetState(EMonsterState NewState, float CurrentTimeOverride);
	void SetTravelTime(float time);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	float GetTravelTime();
	bool IsGarbage();

	UFUNCTION(Category="State")
	void InitMonster();

	// Returns true if blueprint variables were successfully set; otherwise false when they were already setted
	UFUNCTION(BlueprintCallable, Category="Monster")
	bool SetBlueprintVariables(FTransform start_transform, FTransform target_transform, float travel_time);

	UPROPERTY(BlueprintAssignable)
	FOnMonsterDeath OnMonsterDeathDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnMonsterDeath OnNoTargetFoundDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnMonsterStateChanged OnMonsterStateChangedDelegate;
	
	explicit UMonster(const FObjectInitializer& ObjectInitializer);

	void SetIsGarbage(bool b);

protected:
	virtual void HandleRoaming(float DeltaTime);
	virtual void HandleAttack(float DeltaTime);
	virtual void HandleDataStealing(float DeltaTime);
	virtual void HandleShutdown(float DeltaTime);
	virtual void HandleTraveling(float DeltaTime);
	virtual void HandleDeath(float DeltaTime);
	
	UFUNCTION()
	void ReceiveShutdownAlert(ECampus campus);

	float RoamingTime = -1;
	float AttackTime = 20;	// Real value is 20; debug value is 5
	UPROPERTY(BlueprintReadOnly, Category = "Travel", meta=(AllowPrivateAccess=true))
	float TravelingTime = 5;
	bool bIsGarbage = false;
	bool bBlueprintInitialized = false;

};

