// 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSCIFI_API UMeleeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMeleeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ATPSCharacter* Player;

	// Timer handle for controlling fire rate cooldown
	FTimerHandle SwingRateTimerHandle;
	// Timer handle for controlling auto fire
	FTimerHandle AutoSwingTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	UStaticMesh* MeleeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Melee")
	UStaticMeshComponent* MeleeMeshComponent;

	// Sound played when there's a shot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> SwingingSounds;

	// Sound played when trying to shoot but no ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> ImpactSound;

	// Sound played when reloading
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> BlockSound;
	
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void AttachToCharacter(ATPSCharacter* Character);

	void BindWeaponInput(UInputComponent* PlayerInputComponent);

	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void TraceSwing();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual FVector GetMuzzleLocation();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual FRotator GetControlRotation();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void PlaySwingFX();
	
	// Full Auto Fire
	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void AutoSwing();

	// Single Shot per trigger press
	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void SwingCoolDown();

	// Declare the functions that will handle the weapon-specific input
	void StartSwinging();
	void StopSwinging();
	
	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void StartBlocking();

	UFUNCTION(BlueprintCallable, Category = "Melee")
	virtual void StopBlocking();

	// Rate of fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float SwingRate;

	// Damage per shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float Damage;

	// Firing Mode (Single, Auto, Other)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	int SwingMode;
};
