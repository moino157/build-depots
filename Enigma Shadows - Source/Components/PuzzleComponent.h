// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"


enum class ETriggerEvent : uint8;
class APuzzleActor;
class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTMANSION_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPuzzleComponent();

	void BindToPuzzle(APuzzleActor* NewPuzzle);
	void SetupPlayerInputAction(UInputMappingContext* mapping_context,
	UInputAction* directional, UInputAction* cancel);
	virtual void Activate(bool bReset) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// ------------------------  Inputs  ------------------------ //
	void SetupPlayerInputAction();
	virtual void Deactivate() override;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_DirectionalInput;
	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	ETriggerEvent DirectionalTriggerEvent = ETriggerEvent::Triggered;
	void Directional(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_Cancel;

public:
	APuzzleActor* Puzzle;
	void Back();
};
