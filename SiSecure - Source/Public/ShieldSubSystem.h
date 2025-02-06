// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ShieldSubSystem.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCampusStruct
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	float Percentage;
	
	UPROPERTY(BlueprintReadOnly)
	FString CurrentPassword;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsLowDelegateSent;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsShieldDestroyed;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsShutdown;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealthPoints;

	UPROPERTY(BlueprintReadOnly)
	float MaxHealthPoints;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle ShutdownTimerHandle;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLocked;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackCounter;

	FCampusStruct();

	void Reset();
};
UENUM(BlueprintType)
enum class ECampus : uint8 {
	UQAC,
	NAD_SBK,
	NAD_MTL,
	None // adding a  field none to handle return function and null target
};
UENUM(BlueprintType)
enum class EPasswordStrength : uint8 {
	Fort,
	Moyen,
	Faible,
};
ENUM_RANGE_BY_FIRST_AND_LAST(ECampus, ECampus::UQAC, ECampus::NAD_MTL);

UENUM(BlueprintType)
enum class EAttackType : uint8 {
	Shield,
	Node,
	None // adding a  field none to handle server shutdown
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeTimerLow, ECampus, campus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeTimerEnd, ECampus, campus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeTimerReset, ECampus, campus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodeShutdown, ECampus, campus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNodePowerOn, ECampus, campus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDatabaseHealthChanged, float, percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSliderValueChaged, ECampus, campus, float, percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttackedStateChanged, ECampus, campus, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShutdownSliderValueChanged, ECampus, campus, float, percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoDataLeft);

UCLASS()
class CYBERSECURITE_API UShieldSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;

public:

	FTimerDelegate TickDelegateHandle;
	
	UPROPERTY()
	FTimerHandle TickTimerHandle;

	UFUNCTION()
	void Update();
	
	UFUNCTION()
	void UpdateCampusSlider(ECampus campus);

	UFUNCTION()
	void StartSystem();

	UFUNCTION()
	void ResetSystem();
	
	UPROPERTY()
	FCampusStruct UQAC;
	UPROPERTY()
	FCampusStruct NADSBK;
	UPROPERTY()
	FCampusStruct NADMTL;

	UPROPERTY(BlueprintReadWrite)
	ECampus PlayerCurrentPosition = ECampus::None;

	UFUNCTION(BlueprintCallable, Category = "Campus", meta=(DisplayName = "Get Campus_ECampus"), BlueprintPure)
	FCampusStruct& GetCampusStruct_ECampus(ECampus Campus);
	UFUNCTION(BlueprintCallable, Category = "Campus", meta=(DisplayName = "Get Campus_Int"), BlueprintPure)
	FCampusStruct& GetCampusStruct_Int(int CampusIndex);
	UFUNCTION(BlueprintCallable, Category = "Campus", BlueprintPure)
	TArray<FCampusStruct> GetCampusStructs();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int CampusToIndex(ECampus Campus);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static ECampus IndexToCampus(int Index);

	void LockCampus(ECampus campus, bool locked);

	bool CampusIsLocked(ECampus campus);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Password")
	EPasswordStrength GetPasswordStrength_Enum(ECampus campus);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ShutdownScoreCostPerSecond;
	/* ------------------------------ TIMER -----------------------------*/

	// All shutdown timers
	TArray<FTimerHandle> ShutdownTimerHandles;
	// All shutdown Score timers
	TArray<FTimerHandle> ShutdownScoreTimerHandles;

	UFUNCTION()
	void ShutdownScoreUpdate();

	UFUNCTION(BlueprintCallable)
	TArray<ECampus> GetShieldedCampuses();

	//delegate when a shield timer is low
	UPROPERTY(BlueprintAssignable)
	FOnNodeTimerLow OnTimerLowDelegate;

	//delegate when a shield is down
	UPROPERTY(BlueprintAssignable)
	FOnNodeTimerEnd OnTimerEndDelegate;

	//delegate when a shield is back up
	UPROPERTY(BlueprintAssignable)
	FOnNodeTimerReset OnTimerResetDelegate;

	//delegate when a we need to update the slider value
	UPROPERTY(BlueprintAssignable)
	FOnSliderValueChaged OnSliderValueChagedDelegate;
	
	//delegate when we need to update the pie chart value (data base)
	UPROPERTY(BlueprintAssignable)
	FOnDatabaseHealthChanged OnDatabaseHealthChangedDelegate;
	
	//delegate when a play shutdown the server
	UPROPERTY(BlueprintAssignable)
	FOnNodeShutdown OnNodeShutdownDelegate;

	//delegate when a node comes back online
	UPROPERTY(BlueprintAssignable)
	FOnNodePowerOn OnNodePowerOnDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnShutdownSliderValueChanged OnShutdownSliderValueChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttackedStateChanged OnAttackedStateChangedDelegate;

	//function to reset the timer of a node
	UFUNCTION(BlueprintCallable)
	void ResetNodeTimer(ECampus campus);

	// Start timer of a node
	UFUNCTION(BlueprintCallable)
	void ResetHealth(ECampus campus, float rate);

	// called when a timer handle is finished -> broadcast when a campus node shield is destroyed
	UFUNCTION(BlueprintCallable)
	void OnCampusNodeEnd(ECampus campus);

	// apply damage to shield in second return true if damage is on shield and false if on database
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Apply Damage"), Category = "Campus")
	EAttackType ApplyDamageOnCampus(ECampus campus, float damage);
	void ApplyDamageToHealth(float damage);
	// Instantly destroy shield
	UFUNCTION(BlueprintCallable, Category = "Campus")
	void PopShield(ECampus target);

	//function that return an available target
	UFUNCTION(BlueprintCallable)
	ECampus GetAvailableTarget();

	//function that return an available target
	UFUNCTION(BlueprintCallable, Category = "Campus States")
	TArray<ECampus> GetServerStates(bool bReturnDownServer = true);

	// try to shutdown a server. if successful return true;
	UFUNCTION(BlueprintCallable)
	bool ShutdownNode(ECampus campus);
	
	// try to power back on a server. if successful return true;
	UFUNCTION(BlueprintCallable)
	bool PowerNode(ECampus campus);

	UFUNCTION()
	void UpdateCampusTargeting(ECampus campus, bool bIsTargeting);
	
	/* ------------------------------ Data -----------------------------*/
	
	UPROPERTY(BlueprintReadOnly, Category = "Data")
	float DataBaseHealth;

	//delegate when there is no more data left
	UPROPERTY(BlueprintAssignable)
	FOnNoDataLeft OnNoDataLeftDelegate;

	/* ------------------------------ Password -----------------------------*/

	UPROPERTY(BlueprintReadWrite, Category = "Password")
	TArray<FString> oldPasswords;

	UFUNCTION(BlueprintCallable)
	// check if the player input is in the old password data base, then return true if not
	bool ComparePasswordToOld(const FString& Passwords);
	
	UFUNCTION(BlueprintCallable)
	// check if the player input is the last password
	bool ComparePasswordToLast(const FString& Password, ECampus campus);

	UFUNCTION()
	bool CheckIfFirstPasswordsAreAllSet();

	/*-----------------------------Helper function---------------------------------------*/


	UFUNCTION(BlueprintCallable)
	int CampusEnumToIndex(ECampus campus)
	{
		switch (campus)
		{
		case ECampus::NAD_MTL:
			return 0;
		case ECampus::NAD_SBK:
			return 1;
		case ECampus::UQAC:
			return 2;
		case ECampus::None:
			return -1;
		default:
			return -1;
		}
	};

	UFUNCTION(BlueprintCallable)
	ECampus IndexToCampusEnum(int Index)
	{
		if(Index == 0)
		{
			return ECampus::NAD_MTL;
		}
		if(Index == 1)
		{
			return ECampus::NAD_SBK;
		}
		if(Index == 2)
		{
			return ECampus::UQAC;
		}

		return ECampus::None;

	};
	
	
};




