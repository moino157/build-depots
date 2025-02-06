﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMItem.h"
#include "GameFramework/Actor.h"
#include "BMShieldItem.generated.h"

UCLASS()
class BOMBERMAN_API ABMShieldItem : public ABMItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABMShieldItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void ApplyEffect(ABomberManCharacter* BMCharacter) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
