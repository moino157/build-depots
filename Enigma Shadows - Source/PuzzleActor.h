// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractibleActor.h"
#include "PuzzleActor.generated.h"

class UPuzzleComponent;

UCLASS()
class TESTMANSION_API APuzzleActor : public AInteractibleActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuzzleActor();

	virtual bool Interact(AHorrorCharacter* InteractingPlayer) override;
	void Directional(FVector2d input);
	UFUNCTION(BlueprintCallable)
	void PuzzleCompleted();
	UFUNCTION(BlueprintImplementableEvent)
	void TurnHandle(float Turn);
	UFUNCTION(BlueprintImplementableEvent)
	void StartPuzzle();
	UFUNCTION(BlueprintImplementableEvent)
	void StopPuzzle();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPuzzleComponent* Component;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_DirectionalInput;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_Cancel;
};
