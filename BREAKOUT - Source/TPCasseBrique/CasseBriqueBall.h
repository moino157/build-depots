// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CasseBriqueBall.generated.h"

class UNiagaraSystem;
class ATPCasseBriqueGameMode;

UCLASS()
class TPCASSEBRIQUE_API ACasseBriqueBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACasseBriqueBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FVector2D movement_normal = FVector2D(-1,0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float movement_speed = 4.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TArray<USoundBase*> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = VFX)
	UNiagaraSystem* DeathFX;

	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/** Z axis of everything */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Z_Location = 90.0f;

	UPROPERTY()
	ATPCasseBriqueGameMode* CBGameMode;

	FVector2D CalculateBounce(const FVector& IncomingVelocity, const FVector& SurfaceNormal);
};
