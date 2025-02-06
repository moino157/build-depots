// 


#include "GunComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "TPScifi/Characters/Player/TPSCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"
#include "AIController.h"


// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UGunComponent::BeginPlay()
{
    Super::BeginPlay();

    //Player = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UGunComponent::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire FUNCTION"));
    PlayFireFX();
    
    if (CurrentAmmo > 0)
    {
        PlayShootingAnimation();
        // Default firing logic
        if(GetOwner()->GetAttachParentActor())
        {
            //UE_LOG(LogTemp, Warning, TEXT("%s fired by %s"), *GetOwner()->GetName(), *GetOwner()->GetAttachParentActor()->GetName());
            UE_LOG(LogTemp, Warning, TEXT("%s fired by %s"), *GetOwner()->GetName(), *Player->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s fired"), *GetOwner()->GetName());
        }
        
        UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetOwner()->GetActorLocation(), Loudness, GetOwner()->GetOwner(), 0, TEXT("Gunshot"));
        Player->Tags.AddUnique(TEXT("Fire Rate Cooldown"));
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UGunComponent::FireCoolDown, FireRate, false);
        CurrentAmmo--;
        TraceShot();
        UpdateHUD();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
    }
}

void UGunComponent::FireAI()
{
    UE_LOG(LogTemp, Warning, TEXT("FireAI FUNCTION"));
    PlayFireFX();

    if (CurrentAmmo > 0)
    {
        PlayShootingAnimation();
        // Default firing logic
        if (GetOwner()->GetAttachParentActor())
        {
            //UE_LOG(LogTemp, Warning, TEXT("%s fired by %s"), *GetOwner()->GetName(), *GetOwner()->GetAttachParentActor()->GetName());
            UE_LOG(LogTemp, Warning, TEXT("%s fired by %s"), *GetOwner()->GetName(), *Player->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("%s fired"), *GetOwner()->GetName());
        }

        UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetOwner()->GetActorLocation(), Loudness, GetOwner()->GetOwner(), 0, TEXT("Gunshot"));
        Player->Tags.AddUnique(TEXT("Fire Rate Cooldown"));
        GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UGunComponent::FireCoolDown, FireRate, false);
        CurrentAmmo--;
        TraceShot();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Out of ammo!"));
    }
}

void UGunComponent::TraceShot()
{
    UE_LOG(LogTemp, Warning, TEXT("TraceShotAI"));
    // Get the world context
    UWorld* World = GetWorld();
    if (!World) return;

    AController* PC = Cast<AController>(Player->GetController());
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("No Controller found on Player"));
        return;
    }

    FVector CameraLocation;
    FRotator CameraRotation;
    PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

    FVector CameraForwardVector = CameraRotation.Vector();
    FVector RandomForwardVector;
    if (Player->Tags.Contains(TEXT("Player")))
    {
        RandomForwardVector = CameraForwardVector;
    }
    else
    {
        RandomForwardVector = FMath::VRandCone(CameraForwardVector, PI * Alpha / 180);
    }

    // Define a point in front of the player character
    //FVector StartPoint = GetMuzzleLocation();
    FVector StartPoint = CameraLocation;
    FVector EndPoint = CameraLocation + (RandomForwardVector * 10000);

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
                float GivenDamage = Damage;
                bool isHeadshot = HitResult.Component->ComponentTags.Contains("Headshot");
                if(isHeadshot)
                    GivenDamage = Damage * HeadshotDamageMultiplier;
                UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *HitActor->GetName());
                DrawDebugSphere(World, HitResult.ImpactPoint, 10, 20, FColor::Red, false, 0.5, 0, 1);
                UE_LOG(LogTemp, Log, TEXT("Hit Component: %s"), *HitResult.Component->GetName());
                
                // Apply damage or other effects to the hit actor
                UGameplayStatics::ApplyPointDamage(
                    HitActor,
                    GivenDamage,
                    CameraForwardVector,
                    HitResult,
                    GetOwner()->GetInstigatorController(),
                    GetOwner(),
                    UDamageType::StaticClass()
                );

                TraceHitSuccess(isHeadshot, Damage, HitResult);
            }
        }
    }
}

FVector UGunComponent::GetMuzzleLocation()
{
    // Assuming you have a socket or bone named "Muzzle" on your gun's mesh
    if(GunSkeletalMeshComponent)
        return GunSkeletalMeshComponent->GetSocketLocation(FName("Muzzle"));
    else
    return GunMeshComponent->GetSocketLocation(FName("Muzzle"));
}

FRotator UGunComponent::GetControlRotation()
{
    // Get the rotation from the player controller (first-person view)
    if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
    {
        return PC->GetControlRotation();
    }

    // Fallback to the actor's rotation if player controller is not found
    return GetOwner()->GetActorRotation();
}

void UGunComponent::PlayFireFX()
{
    AActor* Character = GetOwner();
    if(Character)
    {
        if(CurrentAmmo > 0)
        {
            // Play sound
            if(ShootingSounds.Num() > 0)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootingSounds[FMath::RandRange(0, ShootingSounds.Num()-1)], Character->GetActorLocation());
            }else
            {
                UE_LOG(LogTemp, Error, TEXT("No shooting sound given for this gun"));
            }
            // Play VFX
        }else
        {
            if(NoAmmoSound)
            {
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), NoAmmoSound, Character->GetActorLocation());
            }else
            {
                UE_LOG(LogTemp, Error, TEXT("No shooting sound given for this gun"));
            }
        }
    }
}

TArray<int> UGunComponent::GetHUDWeaponStats()
{
    TArray<int> StatsArr = {CurrentAmmo, MaxAmmo, SpareAmmo, 0, 0};
    return StatsArr;
}

// Automatically Fire the gun
void UGunComponent::AutoFire()
{
    if(Player->Tags.Contains(TEXT("Trigger Pressed")) && Player->Tags.Contains(TEXT("Reloading")) == false)
    {
        if(CurrentAmmo > 0)
        {
            Player->Tags.AddUnique(TEXT("Fire Rate Cooldown"));
            GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &UGunComponent::FireCoolDown, FireRate, false);
        }
        Fire();
    }
    else
        GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
}

// Single shot
void UGunComponent::FireCoolDown()
{
    Player->Tags.Remove(TEXT("Fire Rate Cooldown"));
}

// Reload the gun
void UGunComponent::Reload()
{
    if(!bIsEquiped)
        return;
    if(MaxAmmo == CurrentAmmo || SpareAmmo == 0)
        return;
    
    if (Player->Tags.Contains(TEXT("Player")))
    {
        int ammo_needed = MaxAmmo - CurrentAmmo;
        if(ammo_needed > SpareAmmo)
        {
            CurrentAmmo = SpareAmmo;
            SpareAmmo = 0;
        }
        else
        {
            CurrentAmmo = MaxAmmo;
            SpareAmmo -= ammo_needed;
        }
    }
    else
    {
        CurrentAmmo = MaxAmmo;
    }
    
    AActor* Character = GetOwner();
    if(Player->Tags.Contains(TEXT("Reloading")) == false)
    {
        PlayReloadingAnimation();
        // Play sound
        if(ReloadSound)
        {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, Character->GetActorLocation());
        }else
        {
            UE_LOG(LogTemp, Error, TEXT("No reload sound given for this gun"));
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Reload end FUNCTION"));
}

void UGunComponent::BindToCharacter(ATPSCharacter* Character)
{
    UE_LOG(LogTemp, Warning, TEXT("Did This call Twice? Owning Actor: %s | Component Name: %s"), *GetOwner()->GetName(),*GetName());
    if (Character)
    {
        Player = Character;
        // Bind weapon input
        if (APlayerController* CharacterController = Cast<APlayerController>(Character->GetController()))
        {
            BindWeaponInput(CharacterController->InputComponent);
        }

        // Attach weapon mesh
        Character->AttachWeapon();
    }
}

void UGunComponent::SetPlayerAsEnemyAI(ATPSCharacter* Character)
{
    Player = Character;
}

void UGunComponent::BindWeaponInput(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(Player->FireAction, ETriggerEvent::Started, this, &UGunComponent::StartShooting);
        EnhancedInputComponent->BindAction(Player->FireAction, ETriggerEvent::Completed, this, &UGunComponent::StopShooting);
        EnhancedInputComponent->BindAction(Player->ReloadAction, ETriggerEvent::Started, this, &UGunComponent::Reload);
    }
}

void UGunComponent::StartShooting()
{
    if(!bIsEquiped)
        return;
    // Handle start shooting logic
    UE_LOG(LogTemp, Warning, TEXT("Base gun StartShooting!"));
    Player->Tags.AddUnique(TEXT("Trigger Pressed"));
    if(Player->Tags.Contains(TEXT("Aiming")) && Player->Tags.Contains(TEXT("Reloading")) == false)
    {
        if(Player->Tags.Contains(TEXT("Fire Rate Cooldown")) == false)
        {
            if(FireMode == 0)   // Single Fire
            {
                Fire();
            }
            else
            {   // Full Auto Fire
                GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, this, &UGunComponent::AutoFire, FireRate, true, 0);
            }
        }
    }
}

void UGunComponent::StopShooting()
{
    if(!bIsEquiped)
        return;
    // Handle stop shooting logic
    Player->Tags.Remove(TEXT("Trigger Pressed"));
    if(Player->Tags.Contains("Aiming") && Player->Tags.Contains("Aim Pressed") == false)
    {
        Player->StopAiming();
    }
}

void UGunComponent::StartShootingAI()
{
    
    if (!bIsEquiped)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI is not shooting"));
        return;
    }
    // Handle start shooting logic
    UE_LOG(LogTemp, Warning, TEXT("AI is shooting"));
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player null"));
        return;
    }
    Player->Tags.AddUnique(TEXT("Trigger Pressed"));
    if (Player->Tags.Contains(TEXT("Aiming")) && Player->Tags.Contains(TEXT("Reloading")) == false)
    {
        if (Player->Tags.Contains(TEXT("Fire Rate Cooldown")) == false)
        {
            UE_LOG(LogTemp, Warning, TEXT("AI is shooting (FIRE_AI)"));
            if (FireMode == 0)   // Single Fire
            {
                Fire();
            }
            else
            {   // Full Auto Fire
                GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, this, &UGunComponent::AutoFire, FireRate, true, 0);
            }
        }
    }
}

void UGunComponent::StopShootingAI()
{
    if (!bIsEquiped)
        return;
    // Handle stop shooting logic
    Player->Tags.Remove(TEXT("Trigger Pressed"));
    if (Player->Tags.Contains("Aiming") && Player->Tags.Contains("Aim Pressed") == false)
    {
        Player->StopAimingAI();
    }
}