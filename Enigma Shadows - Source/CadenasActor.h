// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObservableActor.h"
#include "CadenasActor.generated.h"

UCLASS()
class TESTMANSION_API ACadenasActor : public AObservableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACadenasActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void StopObserving() override;
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveUI();
};
