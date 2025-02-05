// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BMIDestroyable.h"
#include "BMIObstacle.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "BomberManCharacter.generated.h"

class ABMBomb;
class ABMMapGrid;
class UInputAction;

UCLASS(Blueprintable)
class ABomberManCharacter : public ACharacter, public IBMIObstacle, public IBMIDestroyable
{
	GENERATED_BODY()

public:
	ABomberManCharacter();

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_Move;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_Attack;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	void Move(const FInputActionValue& Value);

	// Called when character's bomb explodes
	void CharacterBombExploded();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BombHit() override;

	void SpawnBomb();

	bool bIsShielded = false;
	int MaxBombs = 1;
	int HeldBombs = MaxBombs;
	int BombRange = 2;

	ABMMapGrid* MapRef;

public:

	int player_id = 0;

	UPROPERTY(EditAnywhere, Category = Skin)
	TArray<UMaterialInstance*> CharacterMaterials;
	
	UPROPERTY(EditAnywhere, Category = Sound)
	TArray<USoundBase*> PowerupSounds; 

	void UpgradeBombNumber();
	void UpgradeBombRange();
	void UpgradeShield();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ABMBomb> BombClass;
	
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

