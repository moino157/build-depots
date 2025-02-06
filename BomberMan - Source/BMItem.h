// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMIBuff.h"
#include "GameFramework/Actor.h"
#include "BMItem.generated.h"

UCLASS()
class BOMBERMAN_API ABMItem : public AActor, public IBMIBuff
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABMItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void ApplyEffect(ABomberManCharacter* BMCharacter) override;
};
