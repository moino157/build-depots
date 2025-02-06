// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractibleActor.h"
#include "GameFramework/Actor.h"
#include "ObservableActor.generated.h"

class UObservableItemComponent;

UCLASS()
class TESTMANSION_API AObservableActor : public AInteractibleActor, public IObservableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObservableActor();

protected:
	virtual void BeginPlay() override;
	virtual bool Observe() override;
	
	UPROPERTY(EditAnywhere, Category = "Zoom")
	int min_zoom = 100;
	UPROPERTY(EditAnywhere, Category = "Zoom")
	int max_zoom = 5;
	UPROPERTY(VisibleAnywhere, Category = "Zoom")
	int zoom_level = max_zoom;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	UObservableItemComponent* Component;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_ToggleRotation;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_RotateObject;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_InteractWithObject;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_ZoomObservable;

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_Back;
public:
	UPROPERTY(EditAnywhere, Category = "Controls")
	bool bCanBeRotated = true;
	UPROPERTY(EditAnywhere, Category = "Controls")
	bool bCanBeMoved = true;
	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bViewBlend = false;
	
	virtual bool Interact(AHorrorCharacter* InteractingPlayer) override;
	void RotateObject(FRotator InputRotation);
	void ToggleRotation(bool newToggle);
	void ZoomInOut(float movement);
	virtual void StopObserving();
	UFUNCTION(BlueprintImplementableEvent)
	void InteractWithObject();
};
