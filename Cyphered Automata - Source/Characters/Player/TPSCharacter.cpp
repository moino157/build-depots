// 


#include "TPSCharacter.h"
#include "TPScifi/Weapons/Firearms/PistolComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "TPScifi/Weapons/Firearms/RifleComponent.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSCharacter::OnFireAction()
{
	
}

void ATPSCharacter::OnReloadAction()
{

}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATPSCharacter::StartAiming_Implementation()
{
	Tags.AddUnique(TEXT("Aiming"));
	Tags.AddUnique(TEXT("Aim Pressed"));
	UE_LOG(LogTemp, Warning, TEXT("Aiming!"));
	StartAiming();
}

void ATPSCharacter::StopAimingAction_Implementation()
{
	StopAiming();
}

void ATPSCharacter::StopAiming()
{
	Tags.Remove(TEXT("Aim Pressed"));
	if(Tags.Contains(TEXT("Trigger Pressed")) == false)
	{
		Tags.Remove(TEXT("Aiming"));
		UE_LOG(LogTemp, Warning, TEXT("Stop Aiming!"));
		StopAimingAction();	
	}
}

void ATPSCharacter::HealthRegenStart()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenTickTimerHandle);
	if(CurrentHealth > 0)
		GetWorld()->GetTimerManager().SetTimer(HealthRegenTickTimerHandle, this, &ATPSCharacter::HealthRegenTick, 0.05, true, HealthRegenStartSpeed);
	else
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTimerHandle);
}

void ATPSCharacter::HealthRegenTick()
{
	float target_health = MaxHealth;
	if(CurrentHealth/MaxHealth <= 0.25)
	{
		target_health = MaxHealth*0.25;
	}else if(CurrentHealth/MaxHealth <= 0.5)
	{
		target_health = MaxHealth*0.5;
	}else if(CurrentHealth/MaxHealth <= 0.75)
	{
		target_health = MaxHealth*0.75;
	}

	CurrentHealth += MaxHealth*0.01;
	
	if(CurrentHealth >= target_health)
	{
		CurrentHealth = target_health;
		GetWorld()->GetTimerManager().ClearTimer(HealthRegenTickTimerHandle);
	}
}

void ATPSCharacter::StartAimingAI_Implementation()
{
	Tags.AddUnique(TEXT("Aiming"));
	Tags.AddUnique(TEXT("Aim Pressed"));
	//UE_LOG(LogTemp, Warning, TEXT("AI Aiming"));
	StartAiming();
}

void ATPSCharacter::StopAimingAI_Implementation()
{
	Tags.Remove(TEXT("Aim Pressed"));
	if (!Tags.Contains(TEXT("Trigger Pressed")))
	{
		Tags.Remove(TEXT("Aiming"));
		UE_LOG(LogTemp, Warning, TEXT("AI Stop Aiming!"));
	}
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATPSCharacter::OnFireAction);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ATPSCharacter::OnReloadAction);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ATPSCharacter::StartAiming_Implementation);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATPSCharacter::StopAimingAction_Implementation);
	}
}

