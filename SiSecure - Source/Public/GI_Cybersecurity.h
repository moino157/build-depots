// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "ComplexEmailStruct.h"
#include "NotificationStruct.h"
#include "EmailDifficultyStruct.h"
#include "GamemodeDifficultyStruct.h"
#include "UTimerSystem.h"
#include "GI_Cybersecurity.generated.h"

enum class ECampus : uint8;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverfilledMailbox, FNotificationStruct, content);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmailCreated, FComplexEmailStruct, email, FString, LevelName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateCounter);

USTRUCT(BlueprintType)
struct FTimerWeight
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	int32 Weight;
};

UENUM(BlueprintType)
enum class CampusNodeState : uint8 {
	HEALTHY,
	ATTACKED,
	DOWN
};

UCLASS()
class CYBERSECURITE_API UGI_Cybersecurity : public UGameInstance
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tuto")
	bool HasDoTuto = false;

	float DifficultyModifier = 1;

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category="MailBox")
	TArray<FComplexEmailStruct> UQAC_Mailbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MailBox")
	TArray<FComplexEmailStruct> NADMTL_Mailbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MailBox")
	TArray<FComplexEmailStruct> NADSBK_Mailbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dataset")
	FEmailDifficultyStruct EasyEmailDataSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dataset")
	FEmailDifficultyStruct MediumEmailDataSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dataset")
	FEmailDifficultyStruct ImportantEmailDataSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dataset")
	UDataTable* PlayerTitleMilestoneDataSet;

	// Email Spawn rate curve. Does not take account for the difficulty coefficient
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TArray<FTimerWeight> EmailGenerationFrequencyCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FGamemodeDifficultyStruct EasyGamemodeValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FGamemodeDifficultyStruct MediumGamemodeValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FGamemodeDifficultyStruct HardGamemodeValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int StartingEmailAmount;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	FGamemodeDifficultyStruct CurrentDifficulty;

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	float CurrentEmailGenerationInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Notifications")
	int NumberOfEmailBeforeBossWarning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Notifications")
	int NumberOfEmailBeforeBossCheer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Notifications")
	int MailBoxOverfilledNotification;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Notifications")
	float filledMailboxWarningInterval;

	//Template for the overfilled mailbox notification
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay|Notifications")
	FNotificationStruct OverfilledNotification;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Notifications")
	UTexture2D* UqacLogo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Notifications")
	UTexture2D* MontrealLogo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay|Notifications")
	UTexture2D* SherbrookeLogo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	CampusNodeState UqacNodeState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	CampusNodeState NAD_SBKNodeState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	CampusNodeState NAD_MTLNodeState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool bIsplayerDead = false;

	FTimerHandle EmailGenerationTimerHandle;
	FTimerHandle EmailAmountCheckTimerHandle;

	UPROPERTY(BlueprintAssignable)
	FOnEmailCreated OnEmailCreatedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateCounter OnCountDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnOverfilledMailbox OnOverfilledMailboxDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	ECampus CurrentCampus;


	int BadEmailCount;
	int GoodEmailCount;

	UFUNCTION(BlueprintCallable)
	void SetCurrentCampus(ECampus campus);

	UFUNCTION()
	void AddEmailtoAllMailbox();

	UFUNCTION()
	void CheckIfBoxIsOverfilled();

	UFUNCTION(BlueprintCallable)
	void ChangeDifficulty(FGamemodeDifficultyStruct difficulty);

	UFUNCTION(BlueprintCallable)
	TArray<FComplexEmailStruct>& GetCurrentMailbox();

	UFUNCTION(BlueprintCallable)
	void SpawnStartingEmail();

	UFUNCTION(BlueprintCallable)
	FComplexEmailStruct GenerateEmail();

	UFUNCTION(BlueprintCallable)
	int GetCampusMailBoxCount(ECampus campus);
	
	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, Category = "Password")
	UTimerSystem* TimerSystem;

	UFUNCTION(BlueprintCallable)
	void ResetGame();

	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Password",meta=(ToolTip="Time to generate emails before setting up all the passwords"))
	float PrePasswordEmailGenerationTime;

	

private:
	template <typename T>
	T GetRandomRowFromDataTable(UDataTable* DataTable);

	FTimerHandle DifficultyTimerHandle;


	void SetElementValueWithMin(FComplexEmailStruct& email, int minValue, const FEmailDifficultyStruct& dataset, TSet<int32>& set);
	void SetElementValueWithIndex(FComplexEmailStruct& email, const FEmailDifficultyStruct& dataset, int32 index);
	float GetRandomTimerValueBasedOnWeight();
	FString GetRandomAvailableNode();
	void IncreaseDifficulty();

};

template<typename T>
inline T UGI_Cybersecurity::GetRandomRowFromDataTable(UDataTable* DataTable)
{
	if (!DataTable) return T(); // Return default if the data table is null

	// Get all rows in the data table
	const TArray<FName> RowNames = DataTable->GetRowNames();

	if (RowNames.Num() == 0) return T(); // Return default if no rows

	int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);

	FName RandomRowName = RowNames[RandomIndex];

	T* RandomRow = DataTable->FindRow<T>(RandomRowName, "");

	return RandomRow ? *RandomRow : T();
}
