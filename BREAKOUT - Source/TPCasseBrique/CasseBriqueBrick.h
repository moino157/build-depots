// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CasseBriqueBrick.generated.h"

class ATPCasseBriqueGameMode;
class UNiagaraSystem;

UCLASS()
class TPCASSEBRIQUE_API ACasseBriqueBrick : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACasseBriqueBrick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BrickMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HealthPoints = 1;
	
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* BreakingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	UNiagaraSystem* BreakingFX;

	UPROPERTY(EditAnywhere, Category = Material)
	TArray<UMaterialInstance*> HealthStates;

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY()
	ATPCasseBriqueGameMode* CBGameMode;

	void CheckWinCondition();
};
