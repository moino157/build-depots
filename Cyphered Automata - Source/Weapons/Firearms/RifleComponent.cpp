// 


#include "RifleComponent.h"

// Sets default values for this component's properties
URifleComponent::URifleComponent()
{

}

void URifleComponent::Reload()
{
	Super::Reload();
	UE_LOG(LogTemp, Warning, TEXT("Rifle reloaded!"));
}

// Called when the game starts
void URifleComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void URifleComponent::TickComponent(float DeltaTime, ELevelTick TickType,
									 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

