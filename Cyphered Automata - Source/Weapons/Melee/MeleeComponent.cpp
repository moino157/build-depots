// 


#include "MeleeComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TPScifi/Characters/Player/TPSCharacter.h"

// Sets default values for this component's properties
UMeleeComponent::UMeleeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    // Initialize default values
    SwingRate = 0.2f;
    Damage = 10.0f;
    SwingRate = 0;
}

// Called when the game starts
void UMeleeComponent::BeginPlay()
{
    Super::BeginPlay();

    Player = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UMeleeComponent::Fire()
{
    PlaySwingFX();

    UE_LOG(LogTemp, Warning, TEXT("Melee used!"));
    TraceSwing();
}

void UMeleeComponent::TraceSwing()
{
    // Get the world context
    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PC = Cast<APlayerController>(Player->GetController());
    if(!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("No APlayerController found on Player"));
        return;
    }

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector CameraForwardVector = CameraRotation.Vector();

    // Define a point in front of the player character
    //FVector StartPoint = GetMuzzleLocation();
    FVector StartPoint = CameraLocation;
    FVector EndPoint = CameraLocation + (CameraForwardVector * 10000);

    // Perform the line trace
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner()); // Ignore the gun itself
    CollisionParams.AddIgnoredActor(Player); // Ignore the person holding the gun

    // Perform the line trace from the camera to the calculated end point
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);

    if (bHit)
    {
        StartPoint = GetMuzzleLocation();
        EndPoint = ((HitResult.ImpactPoint - StartPoint).GetSafeNormal()*10000)+StartPoint;
        DrawDebugLine(World, StartPoint, HitResult.ImpactPoint, FColor::Red, false, 0.1f, 0, 1.0f);
        // Perform the line trace from the muzzle to the calculated end point
        bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility, CollisionParams);
        if(bHit)
        {
            // Process hit result
            if (AActor* HitActor = HitResult.GetActor())
            {
                DrawDebugSphere(World, HitResult.ImpactPoint, 10, 20, FColor::Red, false, 0.5, 0, 1);
                UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
                
                // Apply damage or other effects to the hit actor
                UGameplayStatics::ApplyPointDamage(
                    HitActor,
                    Damage,
                    CameraForwardVector,
                    HitResult,
                    GetOwner()->GetInstigatorController(),
                    GetOwner(),
                    UDamageType::StaticClass()
                );
            }
        }
    }
}

FVector UMeleeComponent::GetMuzzleLocation()
{
    // Assuming you have a socket or bone named "Muzzle" on your gun's mesh
    return MeleeMeshComponent->GetSocketLocation(FName("Muzzle"));
}

FRotator UMeleeComponent::GetControlRotation()
{
    // Get the rotation from the player controller (first-person view)
    if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
    {
        return PC->GetControlRotation();
    }

    // Fallback to the actor's rotation if player controller is not found
    return GetOwner()->GetActorRotation();
}

void UMeleeComponent::PlaySwingFX()
{
    AActor* Character = GetOwner();
    if(Character)
    {
        // Play sound
        if(SwingingSounds.Num() > 0)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwingingSounds[FMath::RandRange(0, SwingingSounds.Num()-1)], Character->GetActorLocation());
        }else
        {
            UE_LOG(LogTemp, Error, TEXT("No shooting sound given for this gun"));
        }
        // Play VFX
    }
}

// Automatically Fire the gun
void UMeleeComponent::AutoSwing()
{
    if(Player->Tags.Contains(TEXT("Melee Pressed")))
    {
        Player->Tags.AddUnique(TEXT("Swing Rate Cooldown"));
        GetWorld()->GetTimerManager().SetTimer(SwingRateTimerHandle, this, &UMeleeComponent::SwingCoolDown, SwingRate, false);
        Fire();
    }
}

// Single shot
void UMeleeComponent::SwingCoolDown()
{
    Player->Tags.Remove(TEXT("Swing Rate Cooldown"));
}

void UMeleeComponent::AttachToCharacter(ATPSCharacter* Character)
{
    if (Character)
    {
        // Bind weapon input
        if (APlayerController* CharacterController = Cast<APlayerController>(Character->GetController()))
        {
            BindWeaponInput(CharacterController->InputComponent);
        }

        // Attach weapon mesh
        Character->AttachWeapon();
    }
}

void UMeleeComponent::BindWeaponInput(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(Player->FireAction, ETriggerEvent::Started, this, &UMeleeComponent::StartSwinging);
        EnhancedInputComponent->BindAction(Player->FireAction, ETriggerEvent::Completed, this, &UMeleeComponent::StopSwinging);
        EnhancedInputComponent->BindAction(Player->ReloadAction, ETriggerEvent::Started, this, &UMeleeComponent::StartBlocking);
        EnhancedInputComponent->BindAction(Player->ReloadAction, ETriggerEvent::Completed, this, &UMeleeComponent::StopBlocking);
    }
}

void UMeleeComponent::StartSwinging()
{
    // Handle start shooting logic
    Player->Tags.AddUnique(TEXT("Melee Pressed"));
    if(Player->Tags.Contains(TEXT("Swing Rate Cooldown")) == false)
    {
        if(SwingMode == 0)   // Single Swing per press
        {
            Fire();
        }
        else
        {   // Swing automatically while melee button is pressed
            GetWorld()->GetTimerManager().SetTimer(AutoSwingTimerHandle, this, &UMeleeComponent::AutoSwing, SwingRate, true, 0);
        }
    }
}

void UMeleeComponent::StopSwinging()
{
    Player->Tags.Remove(TEXT("Melee Pressed"));
}

void UMeleeComponent::StartBlocking()
{
    Player->Tags.AddUnique(TEXT("Melee Blocking"));
}

void UMeleeComponent::StopBlocking()
{
    Player->Tags.Remove(TEXT("Melee Blocking"));
}
