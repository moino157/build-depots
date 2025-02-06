// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObservableItemComponent.generated.h"


struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AObservableActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTMANSION_API UObservableItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UObservableItemComponent();

	void BindToObservable(AObservableActor* NewObservable);
	void SetupPlayerInputAction(UInputMappingContext* mapping_context,
		UInputAction* toggle_rotation,
		UInputAction* rotate_object,
		UInputAction* interact_with_object,
		UInputAction* zoom,
		UInputAction* back);
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
	UInputAction* IA_ToggleRotation;
	void ToggleRotation(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_RotateObject;
	void RotateObject(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_InteractWithObject;

	void InteractWithObject(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_ZoomObservable;
	void ZoomInOut(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowPrivateAccess=true))
	UInputAction* IA_Back;
	UFUNCTION(BlueprintCallable, meta=(AllowPrivateAccess=true))
	void Back();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	AObservableActor* CurrentObservable;
};
