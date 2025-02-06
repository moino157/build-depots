// 


#include "PistolComponent.h"


// Sets default values for this component's properties
UPistolComponent::UPistolComponent()
{
	// Initialize specific values for a pistol
	MaxAmmo = 12;
	CurrentAmmo = MaxAmmo;
	FireRate = 0.5f;
	Damage = 15.0f;
	MaxRangeSound = 600;
}

/*void UPistolComponent::Fire()
{
	if (CurrentAmmo > 0)
	{
		// Specific logic for firing a pistol
		UE_LOG(LogTemp, Warning, TEXT("Pistol fired!"));
		CurrentAmmo--;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
	}
}*/

void UPistolComponent::Reload()
{
	Super::Reload();
	UE_LOG(LogTemp, Warning, TEXT("Pistol reloaded!"));
}

// Called when the game starts
void UPistolComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPistolComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

