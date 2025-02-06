// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BMMenuController.generated.h"

struct FInputActionInstance;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class BOMBERMAN_API ABMMenuController : public APlayerController
{
	GENERATED_BODY()

public:
	ABMMenuController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void AddNewPlayer(const FInputActionInstance& ActionInstance);

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_Confirm;
};
