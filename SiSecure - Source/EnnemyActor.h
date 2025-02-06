// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnnemyActor.generated.h"

class UMonster;

UCLASS()
class CYBERSECURITE_API AEnnemyActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, Category = "NPC Subsystem", meta = (AllowPrivateAccess=true))
	UMonster* MonsterUObject;
	
public:
	
};
