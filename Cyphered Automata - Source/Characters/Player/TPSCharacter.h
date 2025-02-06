// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Weapons/Firearms/GunComponent.h"
#include "TPSCharacter.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;

UCLASS()
class TPSCIFI_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;


public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Declare the functions that will handle the input
	void OnFireAction();
	void OnReloadAction();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Gun component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun")
	class UGunComponent* GunComponent;

	// Melee component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee")
	class UGunComponent* MeleeComponent;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon") 
	void AttachWeapon();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StartAiming();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void StartAimingAI();

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	void StopAimingAction();

	void StopAiming();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon")
	void StopAimingAI();

	// Starts the health regen process (launch tick timer)
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void HealthRegenStart();
	// Tick that gives health back to player until it reaches either max health or max section of health
	void HealthRegenTick();

	// Used to regen health after not taking damage for a while
	FTimerHandle HealthRegenTimerHandle;
	// Timer used to add health recursively; called by the health regen timer (eg: +0.1 HP every 0.1 seconds)
	FTimerHandle HealthRegenTickTimerHandle;

	/** -----------------------------------  STATS  -------------------------------------- */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Health")
	float MaxHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Health")
	float CurrentHealth = MaxHealth;
	
	// How long after taking damage does it take to start regaining HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Health")
	float HealthRegenStartSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Mana")
	float Mana = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Armor")
	float Armor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Stealth")
	float Stealth = 0;

	/** -----------------------------------  INPUT ACTIONS  -------------------------------------- */
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Fire (M1) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	
	/** Reload (R) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/** Aim (M2) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Pause Menu (Escape) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PauseMenu;

	/** Change shoulder (V) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ChangeShoulderAction;

	/** Toggle Walk (CAPS LOCK) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ToggleWalkAction;

	/** Toggle Crouch (Left Ctrl) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Start Sprinting (Left Shift) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Jump (Space) Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

};
