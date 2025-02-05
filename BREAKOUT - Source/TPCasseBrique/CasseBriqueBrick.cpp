// Fill out your copyright notice in the Description page of Project Settings.


#include "CasseBriqueBrick.h"

#include "CasseBriqueHUD.h"
#include "NiagaraFunctionLibrary.h"
#include "TPCasseBriqueGameMode.h"
#include "TPCasseBriquePlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACasseBriqueBrick::ACasseBriqueBrick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.AddUnique("Brick");
}

// Called when the game starts or when spawned
void ACasseBriqueBrick::BeginPlay()
{
	Super::BeginPlay();
	// Setting OnHit event
	UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent)
	{
		BrickMesh = MeshComponent;
		// Bind the hit event
		BrickMesh->OnComponentHit.AddDynamic(this, &ACasseBriqueBrick::OnMeshHit);
		// Set material reflecting brick HP
		BrickMesh->SetMaterial(0, HealthStates[HealthPoints-1]);
	}

	// Get reference to GameMode
	CBGameMode = Cast<ATPCasseBriqueGameMode>(GetWorld()->GetAuthGameMode());
	if(!CBGameMode)
		UE_LOG(LogTemp, Error, TEXT("COULD NOT FETCH GAMEMODE"));
	
}

// Called every frame
void ACasseBriqueBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACasseBriqueBrick::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(!OtherActor->Tags.Contains("Ball"))
		return;

	HealthPoints--;
	// Destroy brick if out of health
	if(HealthPoints <= 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BreakingSound);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BreakingFX, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		CBGameMode->IncreaseScore(100);
		CheckWinCondition();
		Destroy();
	}else
	{
		// Change color of brick reflecting health points
		BrickMesh->SetMaterial(0, HealthStates[HealthPoints-1]);
		CBGameMode->IncreaseScore(20);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Brick Hit Detected. New HP: %i"), HealthPoints);
}

void ACasseBriqueBrick::CheckWinCondition()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACasseBriqueBrick::StaticClass(), FoundActors);
	if(FoundActors.Num() == 1)
		CBGameMode->TriggerWin();
}

