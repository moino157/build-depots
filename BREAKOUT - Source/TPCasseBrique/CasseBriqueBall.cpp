// Fill out your copyright notice in the Description page of Project Settings.


#include "CasseBriqueBall.h"

#include "NiagaraFunctionLibrary.h"
#include "TPCasseBriqueGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACasseBriqueBall::ACasseBriqueBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.AddUnique("Ball");
}

// Called when the game starts or when spawned
void ACasseBriqueBall::BeginPlay()
{
	Super::BeginPlay();
	// Setting OnHit event
	UStaticMeshComponent* MeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (MeshComponent)
	{
		BallMesh = MeshComponent;
		// Bind the hit event
		BallMesh->OnComponentHit.AddDynamic(this, &ACasseBriqueBall::OnMeshHit);
		UE_LOG(LogTemp, Warning, TEXT("Ball Mesh collision enabled"));
	}
	// Set the ball to center of screen
	TArray<AActor*> TaggedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("GameCam"), TaggedActors);
	if(TaggedActors.Num() > 0)
	{
		FVector location = TaggedActors[0]->GetActorLocation();
		SetActorLocation(FVector(location.X, location.Y, Z_Location));
	}
	// Get reference to GameMode
	CBGameMode = Cast<ATPCasseBriqueGameMode>(GetWorld()->GetAuthGameMode());
	if(!CBGameMode)
		UE_LOG(LogTemp, Error, TEXT("COULD NOT FETCH GAMEMODE"));
}

// Called every frame
void ACasseBriqueBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Ball movement
	FVector2D result = FVector2D(GetActorLocation().X, GetActorLocation().Y) + (movement_normal * movement_speed);
	SetActorLocation(FVector(result.X, result.Y, Z_Location), true);
}

void ACasseBriqueBall::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Depending on what the ball impacted, a different behavior happens
	FVector ImpactNormal = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
	if(OtherActor->Tags.Contains("Player"))
	{
		movement_normal = FVector2D(ImpactNormal.X, ImpactNormal.Y);
		UGameplayStatics::PlaySound2D(GetWorld(), Sounds[0]);
	}
	else if(OtherActor->Tags.Contains("Death"))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sounds[4]);
		CBGameMode->OnPlayerDead();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathFX, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		Destroy();
	}
	else
	{
		FVector current_velocity = FVector(movement_normal.X, movement_normal.Y, 0);
		movement_normal = CalculateBounce(current_velocity, Hit.Normal);
		// Gotta do multiple if cuz switch cases don't work with FNames *shrug*
		if(OtherActor->Tags.Contains("Brick"))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), Sounds[1]);
			UGameplayStatics::PlaySound2D(GetWorld(), Sounds[3]);
		}
		if(OtherActor->Tags.Contains("Wall"))
			UGameplayStatics::PlaySound2D(GetWorld(), Sounds[2]);
	}
}

FVector2D ACasseBriqueBall::CalculateBounce(const FVector& IncomingVelocity, const FVector& SurfaceNormal)
{
	// Calculate the reflection (bounce) vector
	FVector ReflectedVelocity =  IncomingVelocity - 2 * FVector::DotProduct(IncomingVelocity, SurfaceNormal) * SurfaceNormal;
    
	return FVector2D(ReflectedVelocity.X, ReflectedVelocity.Y).GetSafeNormal();
}

