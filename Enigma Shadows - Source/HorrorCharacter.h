// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HorrorCharacter.generated.h"

class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class TESTMANSION_API AHorrorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHorrorCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float walking_speed = 150;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float running_speed = walking_speed*2;

	UCameraComponent* CameraComponent;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for sprinting input */
	void Sprint(const FInputActionValue& Value);
	
	// ------------------------  Inputs  ------------------------ //
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Sprint;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Flashlight;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Interact;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Use;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_DropItem;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Zoom;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Pause;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Cancel;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Accept;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Inventory;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTransform GetCameraTransform();
	UFUNCTION(BlueprintCallable)
	APlayerController* GetPlayerController() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UInputMappingContext* GetMappingContext() const { return MappingContext; }
};
