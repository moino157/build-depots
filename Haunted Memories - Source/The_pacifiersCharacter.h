// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Logging/LogMacros.h"
#include "Quest/TPQuestComponent.h"
#include "UI/InventoryMenu.h"
#include "The_pacifiersCharacter.generated.h"

class APhysicItem;
class AKeyItem;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AThe_pacifiersCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Camera spring arm */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FirstPersonSpringArmComponent;

	/** Player's flashlight object */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SL_Flashlight;

	/** Camera shakes responsible for head bobbing while walking, running, etc */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UCameraShakeBase>> CameraShakes;

	/** Timeline used for footsteps sounds; works with the material type walked upon (walking on wood makes wood sounds) */
	UPROPERTY(BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* TL_Footstep;

	/** Timeline used for crouch transition */
	UPROPERTY(BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* TL_Crouch;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** -----------------------------------  INPUT ACTIONS  -------------------------------------- */
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Flashlight Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* FlashlightAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction_Pressed;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SprintAction_Released;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	/** Inventory Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;
	/** ------------------------------------------------------------------------------------------- */
	
public:
	AThe_pacifiersCharacter();

protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Used to show line traces made in c++ */
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bEnableTraceDebug = false;

	/** Range of interaction; distance in front of player for interaction trace */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	float InteractionTraceDistance = 150.0f;

	/** Sphere Radius of interaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	float InteractSphereRadius = 30.0f;

	/** Called when player picks up an item */
	UFUNCTION(BlueprintCallable, Category = Item)
	void AddItem(AKeyItem* item);
	
	/** Called when an item is removed from the player's inventory. */
    void RemoveItem(FText item_name);

	/** Called when the item is dropped either by player or if it leaves the range of interaction */
	void DropHeldItem();

	/** Array of items that the player is carrying */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	TArray<AKeyItem*> Items;

	/** Array of notes that the player is carrying */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	TArray<AKeyItem*> Notes;

	UPROPERTY(BlueprintReadWrite, Category = Item)
	APhysicItem* HeldPhysicItem;

	bool bHoldingItem = false;

	UPROPERTY(BlueprintReadWrite, Category = Item)
	bool bInventoryOpened = false;

	/** Returns true if player has item in inventory; otherwise false */
	UFUNCTION(BlueprintCallable, Category = Item)
	AKeyItem* HasItem(FText item_name);
	
	/** Called when an item is used to unlock or consume it; used to decrement amount and remove from inventory */
	UFUNCTION(BlueprintCallable, Category = Item)
	void UseItem(FText item_name);

	/** Event called when the MC uses the item on nothing */
	UFUNCTION(BlueprintNativeEvent, Category = Item)
	void UseItemOnNothing(const FText& Item_Name);

	/** Used to show a message using blueprint that the player no longer needs the item & remove Item from inventory */
	UFUNCTION(BlueprintNativeEvent, Category= Item)
	void DiscardItemMessage(const FText& Item_Name);

	/** Adjust camera lag with high character rotation speed */
	UFUNCTION(BlueprintCallable, Category = Camera)
	void CameraRotation();

	/** Set the appropriate camera shakes depending on player idle, walk or run */
	UFUNCTION(BlueprintCallable, Category = Camera)
	void CameraShake();

	/** Called when the interact button is pressed; returns hit world location */
	void OnInteract();

	/** Verify if player is trying to grab something through a wall */
	bool IsValidHit(FVector HitLocation);
	
	/** Creates a sphere trace to find interactible objects */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	FVector Interact();

	/** Called when the interact button is released */
	void OnInteractReleased();

	/** Creates a sphere trace to find for other actors to use */
	TArray<FHitResult> PlayerSweepTrace(float sphere_radius, float distance, TArray<AActor*> ToIgnore, bool InteractibleOnly = true) const;

	/** Used for lifting physical objects */
	FVector PhysicObjectSweep(float sphere_radius, float distance);

	/** Turn flashlight ON or OFF. bHasFlashlight must be set to true before use. */
	UFUNCTION(BlueprintCallable, Category = Light)
	void ToggleFlashlight();

	/** Executed when the inventory button is pressed. */
	UFUNCTION(BlueprintCallable, Category = Item)
	void ToggleInventory();

	/** Event called when the flashlight is turned on */
	UFUNCTION(BlueprintNativeEvent, Category= Light)
	void OnFlashLightTurnOn();

	/** Event called when the flashlight is turned off */
	UFUNCTION(BlueprintNativeEvent, Category= Light)
	void OnFlashLightTurnOff();

	/** Event called when the flashlight is turned off */
	UFUNCTION(BlueprintNativeEvent, Category= Light)
	void OnNoBatteryTurnOn();

	/** Event called when an item is added to the player's inventory */
	UFUNCTION(BlueprintImplementableEvent, Category= Item)
	void OnInventoryAction();

	/** Called when sprint action is pressed. Will be used in 2 parts in blueprint as retriggerable delays aren't in c++ */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void Sprint_Start();

	/** Called repeatedly when sprinting in BP; Regenerate current stamina by 1; returns false if stamina is full */
	UFUNCTION(BlueprintCallable, Category = Movement)
	bool StaminaRemove();

	/** Called when sprint action is released. Will be used in 2 parts in blueprint as retriggerable delays aren't in c++ */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void Sprint_Stop();

	/** Called repeatedly when sprinting in BP; Reduce current stamina by 1; returns false if stamina is zero */
	UFUNCTION(BlueprintCallable, Category = Movement)
	bool StaminaAdd();

	/** Sets the current stamina of the player. Used in save logic */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void StaminaSet(float NewStamina);

	/** Triggered when crouch is pressed */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void ToggleCrouch();

	/** Called to start crouching */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void Crouch_Start();

	/** Called to stop crouching */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void Crouch_Stop(bool bIsTryingToRun);

	/** Event used to test in blueprint if the PC can stand up */
	UFUNCTION(BlueprintNativeEvent, Category= Light)
	void OnCrouchStopCalled(bool bIsTryingToRun);

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called once when the move action is pressed */
	void Move_Start();

	/** Called once when the move action is released */
	void Move_Stop();

	/** Player's crouch speed */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float crouch_speed = 100.0;

	/** Player's walk speed */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float walk_speed = 300.0;

	/** Player's run speed */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float run_speed = 500.0;

	/** Player's max stamina. Setting this to zero prevents the player from running. */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float stamina_max = 100.0;

	/** Player's current stamina */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float stamina_current = 100.0;

	/** True if player is running; otherwise false */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsRunning = false;

	/** True if player is crouching */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsCrouching = false;

	/** The height of the capsule when standing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shape)
	float capsule_half_height_standing = 96.0;

	/** The height of the capsule when crouching */
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Shape)
	float capsule_half_height_crouching = 50.0;

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadOnly, Category = Light)
	bool bIsFlashlightOn = false;

	UPROPERTY(BlueprintReadWrite, Category = Light)
	bool bHasBattery = true;

	/** Godmode. Infinite stamina and can't be killed. Used for debugging */
	UPROPERTY(EditAnywhere, Category = Debug)
	bool bGodMode = false;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UInventoryMenu* InventoryMenu;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Quest")
	UTpQuestComponent* QuestComponent;
};

