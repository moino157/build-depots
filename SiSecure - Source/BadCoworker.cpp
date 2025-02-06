// Fill out your copyright notice in the Description page of Project Settings.


#include "BadCoworker.h"

#include "GI_Cybersecurity.h"
#include "ShieldSubSystem.h"

UBadCoworker::UBadCoworker(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Type = EMonsterType::CoWorker;
	TravelingTime = 60;	// 60 seconds to get to node
}

float UBadCoworker::GetSpawnTime(UGI_Cybersecurity* gi)
{
	float scaled_spawn_time = FMath::Clamp(SpawnTime*(2-gi->DifficultyModifier), 30, SpawnTime*3);
	return scaled_spawn_time;
}

void UBadCoworker::HandleAttack(float DeltaTime)
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
	UShieldSubSystem* ShieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

	ShieldSubSystem->PopShield(Target);
	ShieldSubSystem->ApplyDamageToHealth(0.1);

	SetState(EMonsterState::Death);
}

void UBadCoworker::HandleShutdown(float DeltaTime)
{
	SetState(EMonsterState::Death);
}
