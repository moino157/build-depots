// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCSubSystem.h"

#include "GI_Cybersecurity.h"
#include "ShieldSubSystem.h"
#include "Cybersecurite/BadCoworker.h"
#include "Cybersecurite/BruteForce.h"

void UNPCSubSystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NPC sub system initialized");
}

UMonster* UNPCSubSystem::SpawnMonsterNoTarget(EMonsterType type)
{
	bFirstMonsterSpawned = true;
	UMonster* NewMonster;
	switch (type)
	{
	case EMonsterType::BruteForce:
		if(GetMonsterTypeCount(type) >= 3)
			return NULL;
		NewMonster = NewObject<UBruteForce>(this, UBruteForce::StaticClass());
		break;
	case EMonsterType::CoWorker:
		NewMonster = NewObject<UBadCoworker>(this, UBadCoworker::StaticClass());
		break;
	default:
		NewMonster = NewObject<UBruteForce>(this, UBruteForce::StaticClass());
		break;
	}
	Monsters.Add(NewMonster);
	NewMonster->InitMonster();
	OnMonsterSpawnDelegate.Broadcast(NewMonster);
	
	NewMonster->OnMonsterAttackStateChangedDelegate.AddDynamic(this, &UNPCSubSystem::HandleMonsterStateChange);
	UE_LOG(LogTemp, Log, TEXT("A new monster of type %hhd is spawned"), NewMonster->Type);
	return NewMonster;
}

UMonster* UNPCSubSystem::SpawnMonster(EMonsterType type, ECampus target)
{
	bFirstMonsterSpawned = true;
	UMonster* NewMonster;
	switch (type)
	{
	case EMonsterType::BruteForce:
		if(GetMonsterTypeCount(type) >= 3)
			return NULL;
		NewMonster = NewObject<UBruteForce>(this, UBruteForce::StaticClass());
		break;
	case EMonsterType::CoWorker:
		NewMonster = NewObject<UBadCoworker>(this, UBadCoworker::StaticClass());
		break;
	default:
		NewMonster = NewObject<UBruteForce>(this, UBruteForce::StaticClass());
		break;
	}
	NewMonster->Target = target;
	Monsters.Add(NewMonster);
	NewMonster->InitMonster();
	OnMonsterSpawnDelegate.Broadcast(NewMonster);
	
	NewMonster->OnMonsterAttackStateChangedDelegate.AddDynamic(this, &UNPCSubSystem::HandleMonsterStateChange);
	
	UE_LOG(LogTemp, Log, TEXT("A new monster of type %hhd is spawned"), NewMonster->Type);
	return NewMonster;
}

UMonster* UNPCSubSystem::SpawnMonsterWithState(EMonsterType type, ECampus target, EMonsterState state)
{
	bFirstMonsterSpawned = true;
	UMonster* NewMonster;
	switch (type)
	{
	case EMonsterType::BruteForce:
		if(GetMonsterTypeCount(type) >= 3)
			return NULL;
		NewMonster = NewObject<UBruteForce>(this, UBruteForce::StaticClass());
		break;
	case EMonsterType::CoWorker:
		NewMonster = NewObject<UBadCoworker>(this, UBadCoworker::StaticClass());
		break;
	default:
		NewMonster = NewObject<UBruteForce>(this, UBruteForce::StaticClass());
		break;
	}
	NewMonster->Target = target;
	NewMonster->SetState(state);
	Monsters.Add(NewMonster);
	NewMonster->InitMonster();
	OnMonsterSpawnDelegate.Broadcast(NewMonster);

	NewMonster->OnMonsterAttackStateChangedDelegate.AddDynamic(this, &UNPCSubSystem::HandleMonsterStateChange);
	UE_LOG(LogTemp, Log, TEXT("A new monster of type %hhd is spawned"), NewMonster->Type);
	return NewMonster;
}

void UNPCSubSystem::SpawnMonsterDelayed(EMonsterType type)
{
	switch (type)
	{
	case EMonsterType::BruteForce:
		SpawnMonsterNoTarget(type);
		break;
	case EMonsterType::CoWorker:
		ECampus CoworkerTarget = GetCoworkerTargetCampus();
		if(CoworkerTarget != ECampus::None)
		{	// Starts next timer
			SpawnMonsterWithState(EMonsterType::CoWorker
				, CoworkerTarget
				, EMonsterState::Traveling
			);
		}
		// Handles starting countdown until bad coworker spawns
		FTimerDelegate DelayedMonsterSpawnDelegate;
		DelayedMonsterSpawnDelegate.BindUFunction(this, FName("SpawnMonsterDelayed"), EMonsterType::CoWorker);
		GetWorld()->GetTimerManager().SetTimer(
		BadCoworkerSpawnTimer,
		DelayedMonsterSpawnDelegate,
		UBadCoworker::GetSpawnTime(Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance())),
		false
		);
		break;
	}
}

void UNPCSubSystem::HandleMonsterStateChange(EMonsterState PreviousState, EMonsterState CurrentState, UMonster* Object)
{
	UGI_Cybersecurity* GameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
	UShieldSubSystem* ShieldSubSystem = GameInstancePtr->GetSubsystem<UShieldSubSystem>();

	bool WasTargeting = IsTargetingState(PreviousState);
	bool IsNowTargeting = IsTargetingState(CurrentState);

	if (!WasTargeting && IsNowTargeting)
	{
		// Monster started targeting a campus
		ShieldSubSystem->UpdateCampusTargeting(Object->Target, true);
	}
	else if (WasTargeting && !IsNowTargeting)
	{
		// Monster stopped targeting a campus
		ShieldSubSystem->UpdateCampusTargeting(Object->Target, false);
	}
}

bool UNPCSubSystem::IsTargetingState(EMonsterState State)
{
	return State == EMonsterState::Attack || State == EMonsterState::DataStealing;
}

ECampus UNPCSubSystem::GetCoworkerTargetCampus()
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
	UShieldSubSystem* ShieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();
	
	TArray<ECampus> campuses = { ECampus::NAD_MTL, ECampus::NAD_SBK, ECampus::UQAC};
	FCampusStruct campus_data;
	do
	{
		int index = FMath::RandRange(0,campuses.Num()-1);
		campus_data = ShieldSubSystem->GetCampusStruct_ECampus(campuses[index]);
		if(campus_data.bIsShutdown)
			campuses.RemoveAt(index);
		else
			return campuses[index];
	}while(campuses.Num() > 0);
	return ECampus::None;
}

bool UNPCSubSystem::RollMonsterSpawn(ECampus PlayerLocation, EMonsterType type)
{
	UGI_Cybersecurity* gi = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
	switch (type)
	{
	case EMonsterType::BruteForce:
		if(FMath::FRandRange(0.0f, 1.0f) <= UBruteForce::GetSpawnChance(gi))
		{
			UE_LOG(LogTemp, Log, TEXT("Chance roll for monster spawn was %f"), UBruteForce::GetSpawnChance(gi));
			SpawnMonsterWithState(EMonsterType::BruteForce, PlayerLocation, EMonsterState::Traveling);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green
				, TEXT("Player mistake caused a BruteForce to spawn."));
			return true;
		}
		break;
	}
	return false;
}

int UNPCSubSystem::GetMonsterTypeCount(EMonsterType type)
{
	int count = 0;
	for(UMonster* Monster : Monsters)
	{
		if(Monster->Type == type)
			count++;
	}
	return count;
}

void UNPCSubSystem::DestroyMonster(UMonster* Monster)
{
	if (Monster)
	{
		// delegate called when the timer is finished
		FTimerDelegate DelayedMonsterSpawnDelegate;
		DelayedMonsterSpawnDelegate.BindUFunction(this, FName("SpawnMonsterDelayed"), EMonsterType::BruteForce);

		// if this is the last BruteForce that is being destroyed
		if(Monster->Type == EMonsterType::BruteForce
			&& GetMonsterTypeCount(EMonsterType::BruteForce) <= 1)
		{
			UGI_Cybersecurity* gi = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
			const float next_spawn_time = UBruteForce::GetSpawnTime(gi);
			UE_LOG(LogTemp, Log, TEXT("Next brute force will spawn in %f"), next_spawn_time);
			// Will spawn a new bruteforce after 30 to 60 seconds
			GetWorld()->GetTimerManager().SetTimer(
				BruteForceSpawnTimer,
				DelayedMonsterSpawnDelegate,
				next_spawn_time,
				false);
		}
		

		// marks the object for garbage collection)
		// Monster->MarkAsGarbage();
	}
}

const TArray<UMonster*>& UNPCSubSystem::GetMonsters() const
{
	return Monsters;
}

bool UNPCSubSystem::HasFirstMonsterSpawned()
{
	return bFirstMonsterSpawned;
}

void UNPCSubSystem::UpdateMonsters()
{
	TArray<UMonster*> GarbageMonsters;
	for (UMonster* Monster : Monsters)
	{
		if(Monster->IsGarbage())
			GarbageMonsters.Add(Monster);
		else
			Monster->Update(UpdateInterval);
	}
	// Clean out garbage monsters from monster list
	while(GarbageMonsters.Num() != 0)
	{
		// destroy the uobject
		for(auto m : GarbageMonsters)
		{
			m->MarkAsGarbage();
		}

		//removing it from the list
		Monsters.Remove(GarbageMonsters.Pop());
	}
}

void UNPCSubSystem::ResetSystem()
{
	//clear update timer first
	GetWorld()->GetTimerManager().ClearTimer(MonsterUpdateTimerHandle);
	
	//destroy all monsters
	for (auto m : Monsters)
	{
		m->MarkAsGarbage();
	}

	Monsters.Empty();

	// clear all timers

	GetWorld()->GetTimerManager().ClearTimer(BruteForceSpawnTimer);
	GetWorld()->GetTimerManager().ClearTimer(BadCoworkerSpawnTimer);

	bFirstMonsterSpawned = false;
}

void UNPCSubSystem::StartSystem()
{
	// Set the timer to repeatedly call UpdateMonsters() at the defined interval
	GetWorld()->GetTimerManager().SetTimer(
		MonsterUpdateTimerHandle, 
		this, 
		&UNPCSubSystem::UpdateMonsters, 
		UpdateInterval, 
		true  // true means it loops
	);
	
	// Handles starting countdown until bad coworker spawns
	FTimerDelegate DelayedMonsterSpawnDelegate;
	DelayedMonsterSpawnDelegate.BindUFunction(this, FName("SpawnMonsterDelayed"), EMonsterType::CoWorker);
	GetWorld()->GetTimerManager().SetTimer(
		BadCoworkerSpawnTimer,
		DelayedMonsterSpawnDelegate,
		UBadCoworker::GetSpawnTime(Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance())),
		false);
}
