// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunComponent.generated.h"


struct FEnhancedInputActionEventBinding;
class ATPSCharacter;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSCIFI_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGunComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ATPSCharacter* Player;

	// Timer handle for controlling fire rate cooldown
	FTimerHandle FireRateTimerHandle;
	// Timer handle for controlling auto fire
	FTimerHandle AutoFireTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	UStaticMesh* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	USkeletalMesh* GunSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
	UStaticMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Gun")
	USkeletalMeshComponent* GunSkeletalMeshComponent;

	// Sound played when there's a shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TArray<USoundBase*> ShootingSounds;

	// Sound played when trying to shoot but no ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* NoAmmoSound;

	// Sound played when reloading
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ReloadSound;
	
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void BindToCharacter(ATPSCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void SetPlayerAsEnemyAI(ATPSCharacter* Character);

	void BindWeaponInput(UInputComponent* PlayerInputComponent);

	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual void FireAI();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual void TraceShot();

	/** Function called when a bullet hits something */
	UFUNCTION(BlueprintImplementableEvent, Category = "Gun")
	void TraceHitSuccess(bool bIsHeadShot, float fBaseDamage, FHitResult HitResult);

	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual FVector GetMuzzleLocation();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual FRotator GetControlRotation();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void PlayFireFX();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gun")
	void PlayShootingAnimation();

	UFUNCTION(BlueprintCallable, Category = "HUD | Gun")
	TArray<int> GetHUDWeaponStats();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD | Gun")
	void UpdateHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gun")
	void PlayReloadingAnimation();
	
	// Full Auto Fire
	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual void AutoFire();

	// Single Shot per trigger press
	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual void FireCoolDown();

	// Declare the functions that will handle the weapon-specific input
	UFUNCTION(BlueprintCallable, Category = "Gun")
	void StartShooting();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void StopShooting();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void StartShootingAI();

	UFUNCTION(BlueprintCallable, Category = "Gun")
	void StopShootingAI();

	// Reload the gun
	UFUNCTION(BlueprintCallable, Category = "Gun")
	virtual void Reload();

	// Ammo count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int32 CurrentAmmo = 30;

	// Ammo in a magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int32 MaxAmmo = CurrentAmmo;

	// Ammo in inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int32 SpareAmmo;
	
	// Rate of fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float FireRate = 0.2;

	// Damage per shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float Damage = 22.0f;

	// Factor to multiply damage when headshot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float HeadshotDamageMultiplier = 1.5f;

	// Distance which NPC can hear your shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float Loudness = 5000.0f;
	
	// FOV divider when aiming to zoom cam
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float AimZoom = 1.0f;

	// Firing Mode (Single, Auto, Other)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	int FireMode = 0;

	// Firing Mode (Single, Auto, Other)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	bool bIsEquiped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float MaxRangeSound = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
	float Alpha = 15.0f;
};