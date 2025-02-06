// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComplexEmailStruct.h"
#include "GI_Cybersecurity.h"
#include "NotificationStruct.h"
#include "UW_EmailScreen.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnswer, bool, answer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotificationSent, FNotificationStruct, content);
/**
 * 
 */
UCLASS()
class CYBERSECURITE_API UUW_EmailScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentEmailIndex;

	//Template for the anti virus notification
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNotificationStruct AntivirusNotification;

	//Template for the positive notification the boss is sending
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNotificationStruct BossPositiveNotification;

	//Template for the negative notification the boss is sending
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNotificationStruct BossNegativeNotification;

	//Template for the negative notification the boss is sending when a link is ignored
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNotificationStruct BosslinkIgnoredNotification;


	FTimerHandle EmailTimerHandle;
	FTimerHandle EvaluationTimerHandle;

	UFUNCTION(BlueprintCallable)
	void OnScamButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnVerifiedButtonClicked();

	UPROPERTY(BlueprintAssignable)
	FOnAnswer OnAnswerDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnNotificationSent OnNotificationSentDelegate;

	UFUNCTION(BlueprintCallable)
	bool OnHyperlinkClicked();

	UFUNCTION(BlueprintCallable)
	ECampus GetComputerLockScreen();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHyperlinkedClicked = false;


protected:
	
	int CorrectEmailsFiltered = 0;
	int IncorrectEmailsFiltered = 0;
	TArray<bool> LastTenFilteredResults;

	UFUNCTION()
	void CheckEmail(bool bClickedScam);
	
};
