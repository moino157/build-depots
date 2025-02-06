// Fill out your copyright notice in the Description page of Project Settings.


#include "BruteForce.h"

#include "GI_Cybersecurity.h"
#include "ShieldSubSystem.h"


UBruteForce::UBruteForce(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Type = EMonsterType::BruteForce;
}

float UBruteForce::GetSpawnTime(UGI_Cybersecurity* gi)
{
	float scaled_spawn_time = FMath::Clamp(SpawnTime*(2-gi->DifficultyModifier), 5, 60*3);
	return scaled_spawn_time;
}

float UBruteForce::GetSpawnChance(UGI_Cybersecurity* gi)
{
	float scaled_spawn_chance = FMath::Clamp(SpawnChance*gi->DifficultyModifier,0,1);
	return scaled_spawn_chance;
}

void UBruteForce::HandleRoaming(float DeltaTime)
{
	if(RoamingTime < 0)
		RoamingTime = FMath::RandRange(15, 180);
	
	//need to implement roaming
	// roaming for 15 to 180 seconds then traveling to attack a node (15 set for debug)
	if(CurrentStateTime >= RoamingTime)
	{
		UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
		UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

		Target = shieldSubSystem->GetAvailableTarget();
		SetState(EMonsterState::Traveling);
	}
}

void UBruteForce::HandleAttack(float DeltaTime)
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
	UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

	// stop attacking after attack time and travels despawn
	if(CurrentStateTime < AttackTime)
	{
		// check if the monster is attacking the shield or the database
		EAttackType result = shieldSubSystem->ApplyDamageOnCampus(Target, 0.01f);
		if(result == EAttackType::Node)
			SetState(EMonsterState::DataStealing, CurrentStateTime);
	}
	else
	{
		SetState(EMonsterState::Death);
	}

}

void UBruteForce::HandleDataStealing(float DeltaTime)
{
	//need to implement stealing
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
	UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

	// Keeps attacking until exceeding attack time
	if(CurrentStateTime < AttackTime)
	{
		// check if the monster is attacking the shield or the database
		EAttackType result = shieldSubSystem->ApplyDamageOnCampus(Target, 0.01f);
		// if the shield is back up then return to attack
		if(result == EAttackType::Shield)
		{
			SetState(EMonsterState::Attack);
		}
		// if the server is shutdown
		else if(result == EAttackType::None)
		{
			SetState(EMonsterState::Shutdown);
		}
	}else
	{
		SetState(EMonsterState::Death);
	}

}

void UBruteForce::HandleShutdown(float DeltaTime)
{
	SetState(EMonsterState::Death);
}
