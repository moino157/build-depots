// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMIObstacle.h"
#include "GameFramework/Actor.h"
#include "BMWall.generated.h"

UCLASS()
class BOMBERMAN_API ABMWall : public AActor, public IBMIObstacle
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABMWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
};
