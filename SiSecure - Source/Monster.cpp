// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "GI_Cybersecurity.h"
#include "NPCSubSystem.h"
#include "ShieldSubSystem.h"

class UShieldSubSystem;
UMonster::UMonster(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	//spawning monster in roaming
	CurrentState = EMonsterState::Roaming;
	CurrentStateTime = 0;
	Target = ECampus::None;
}

void UMonster::SetIsGarbage(bool b)
{
	bIsGarbage = b;
}


void UMonster::Update(float DeltaTime)
{

	// to keep track of the monster position and state transition time
	CurrentStateTime += DeltaTime;
	
	switch (CurrentState)
	{
	case EMonsterState::Roaming:
		HandleRoaming(DeltaTime);
		break;
	case EMonsterState::Attack:
		HandleAttack(DeltaTime);
		break;
	case EMonsterState::DataStealing:
		HandleDataStealing(DeltaTime);
		break;
	case EMonsterState::Traveling:
		HandleTraveling(DeltaTime);
		break;
	case EMonsterState::Shutdown:
		HandleShutdown(DeltaTime);
		break;
	case EMonsterState::Death:
		HandleDeath(DeltaTime);
		break;;
	default:
		break;
	}
}

void UMonster::SetState(EMonsterState NewState)
{
	UE_LOG(LogTemp, Log, TEXT("New state for monster: %hhd"), NewState);
	PreviousState = CurrentState;
	CurrentState = NewState;
	CurrentStateTime = 0;
	OnMonsterStateChangedDelegate.Broadcast(NewState);
	OnMonsterAttackStateChangedDelegate.Broadcast(PreviousState, CurrentState, this);
}

void UMonster::SetState(EMonsterState NewState, float CurrentTimeOverride)
{
	UE_LOG(LogTemp, Log, TEXT("New state for monster: %hhd"), NewState);
	PreviousState = CurrentState;
	CurrentState = NewState;
	CurrentStateTime = CurrentTimeOverride;
	OnMonsterStateChangedDelegate.Broadcast(NewState);
	OnMonsterAttackStateChangedDelegate.Broadcast(PreviousState, CurrentState, this);
}

void UMonster::SetTravelTime(float time)
{
	UGI_Cybersecurity* gi = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
	float Scaling = FMath::Clamp(gi->DifficultyModifier, 1, 5);
	TravelingTime = time/Scaling;
}

float UMonster::GetTravelTime()
{
	return TravelingTime;
}

bool UMonster::IsGarbage()
{
	return bIsGarbage;
}

void UMonster::InitMonster()
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
	UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

	// bind alert when a server is shutdown
	if(shieldSubSystem)
		shieldSubSystem->OnNodeShutdownDelegate.AddDynamic(this, &UMonster::ReceiveShutdownAlert);
	else
		UE_LOG(LogTemp, Error, TEXT("Could not init monster properly. Unable to bind delegate"));
}

bool UMonster::SetBlueprintVariables(FTransform start_transform, FTransform target_transform, float travel_time)
{
	if(bBlueprintInitialized)
		return false;

	StartTranform = start_transform;
	TargetTranform = target_transform;
	SetTravelTime(travel_time);
	bBlueprintInitialized = true;
	return true;
}

void UMonster::HandleRoaming(float DeltaTime)
{
	UE_LOG(LogTemp, Error, TEXT("Monster specific behaviors were moved to children classes. This function needs to be overritten"));
}

void UMonster::HandleAttack(float DeltaTime)
{
	UE_LOG(LogTemp, Error, TEXT("Monster specific behaviors were moved to children classes. This function needs to be overritten"));
}

void UMonster::HandleDataStealing(float DeltaTime)
{
	UE_LOG(LogTemp, Error, TEXT("Monster specific behaviors were moved to children classes. This function needs to be overritten"));
	
}

void UMonster::HandleShutdown(float DeltaTime)
{
	//need to implement shutdown
}

void UMonster::HandleTraveling(float DeltaTime)
{
	// move to a node
	if(Target != ECampus::None)
	{
		if(CurrentStateTime >= TravelingTime)
		{
			SetState(EMonsterState::Attack);
		}

	}
	// move outside the map
	else
	{
		OnNoTargetFoundDelegate.Broadcast();
	}
}

void UMonster::HandleDeath(float DeltaTime)
{
	OnMonsterDeathDelegate.Broadcast();
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(UObject::GetWorld()->GetGameInstance());
	UNPCSubSystem* NPCSubSystem = gameInstancePtr->GetSubsystem<UNPCSubSystem>();

	bIsGarbage = true;
	NPCSubSystem->DestroyMonster(this);
}

void UMonster::ReceiveShutdownAlert(ECampus campus)
{
	UE_LOG(LogTemp, Log, TEXT("Received shutdown alert"));
	// If the monster's target becomes shutdown
	if(Target == campus)
	{
		SetState(EMonsterState::Shutdown);
	}

}
