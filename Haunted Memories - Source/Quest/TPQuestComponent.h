// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPQuestDataAsset.h"
#include "Components/ActorComponent.h"
#include "TPQuestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestUpdate,FText,QuestName,FText,QuestDescription);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THE_PACIFIERS_API UTpQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTpQuestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
	int currentQuestId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTPQuestDataAsset* QuestDataAsset;

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdate OnQuestUpdate;

	TArray<bool> QuestsValidated;

public:

	UFUNCTION(BlueprintCallable)
	void UpdateQuest();

	UFUNCTION(BlueprintCallable)
	void ValidateQuest(int questId);
	
};
