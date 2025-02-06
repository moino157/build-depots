// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldSubSystem.h"
#include "Password_Widget.h"
#include "GI_Cybersecurity.h"
#include "ScoreSubSystem.h"
#include "Password_Widget.h"
#include "Kismet/GameplayStatics.h"

FCampusStruct::FCampusStruct(): Percentage(1), CurrentPassword(TEXT("")), bIsLowDelegateSent(false), bIsShieldDestroyed(false), bIsShutdown(false), bIsLocked(false), CurrentHealthPoints(0), MaxHealthPoints(0), AttackCounter(0)
{
}

void FCampusStruct::Reset()
{
	Percentage = 1;
	CurrentPassword = TEXT("");
	bIsLowDelegateSent = false;
	bIsShieldDestroyed = false;
	bIsShutdown = false;
	bIsLocked = false;
	CurrentHealthPoints = 0;
	MaxHealthPoints = 0;
	AttackCounter = 0;
}

void UShieldSubSystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	Super::Initialize(collection);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Shield sub system initialized");
	ShutdownScoreCostPerSecond = 1;
}

void UShieldSubSystem::StartSystem()
{
	// creating strong shield at the start of a game
	ResetHealth(ECampus::UQAC, 10);
	ResetHealth(ECampus::NAD_MTL, 10);
	ResetHealth(ECampus::NAD_SBK, 10);

	// Creates 3 indexes for each Shutdown Timers
	ShutdownTimerHandles.SetNum(3);
	ShutdownScoreTimerHandles.SetNum(3);
	
	DataBaseHealth = 1;


	// handle shutdown nodes
	TickDelegateHandle.BindUFunction(this, FName("Update"));
	
	// simulate update at 60 fps for ui
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, TickDelegateHandle, 1/60.0f, true);
}

void UShieldSubSystem::ResetSystem()
{
	// Clear update timer
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);

	// Clear shutdown timers
	for (auto TimerHandle : ShutdownTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	//clear shutdown score timers
	for (auto TimerHandle : ShutdownScoreTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	// reset campus nodes 
	UQAC.Reset();
	NADMTL.Reset();
	NADSBK.Reset();

	// reset used passwords
	oldPasswords.Empty();

	
}


void UShieldSubSystem::Update()
{

	for (int i = 0; i < ShutdownTimerHandles.Num(); i++)
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(ShutdownTimerHandles[i]))
		{
			float p = GetWorld()->GetTimerManager().GetTimerRemaining(ShutdownTimerHandles[i])/GetWorld()->GetTimerManager().GetTimerRate(ShutdownTimerHandles[i]); 
			OnShutdownSliderValueChangedDelegate.Broadcast(IndexToCampus(i), p);
		}
	}
}


void UShieldSubSystem::UpdateCampusSlider(ECampus campus)
{
	
	FCampusStruct& Cstruct = GetCampusStruct_ECampus(campus);

	Cstruct.Percentage = Cstruct.CurrentHealthPoints/Cstruct.MaxHealthPoints;
	
	//threshold when timer is low
	if(Cstruct.Percentage <= 0.3f && !Cstruct.bIsLowDelegateSent)
	{
		OnTimerLowDelegate.Broadcast(campus);
		Cstruct.bIsLowDelegateSent = true;
	}

	if(Cstruct.Percentage <= 0)
	{
		Cstruct.Percentage = 0;
		Cstruct.bIsShieldDestroyed = true;
		
		// still calling the delegate event if the timer is not finished
		OnCampusNodeEnd(campus);
	}

	OnSliderValueChagedDelegate.Broadcast(campus, Cstruct.Percentage);
	

}

FCampusStruct& UShieldSubSystem::GetCampusStruct_ECampus(ECampus Campus)
{
	switch (Campus)
	{
	case ECampus::UQAC:
		return UQAC;
	case ECampus::NAD_MTL:
		return NADMTL;
	case ECampus::NAD_SBK:
		return NADSBK;
	default:
		static FCampusStruct defaultStruct;
		return defaultStruct;
	}
}

FCampusStruct& UShieldSubSystem::GetCampusStruct_Int(int CampusIndex)
{
	return GetCampusStruct_ECampus(IndexToCampus(CampusIndex));
}

TArray<FCampusStruct> UShieldSubSystem::GetCampusStructs()
{
	TArray<FCampusStruct> Campuses = {NADMTL, NADSBK, UQAC};
	return Campuses;
}

int UShieldSubSystem::CampusToIndex(ECampus Campus)
{
	switch (Campus)
	{
	case ECampus::NAD_MTL:
		return 0;
	case ECampus::NAD_SBK:
		return 1;
	case ECampus::UQAC:
		return 2;
	default:
		return -1;
	}
}

ECampus UShieldSubSystem::IndexToCampus(int Index)
{
	switch (Index)
	{
	case 0:
		return ECampus::NAD_MTL;
	case 1:
		return ECampus::NAD_SBK;
	case 2:
		return ECampus::UQAC;
	default:
		return ECampus::None;
	}
}

void UShieldSubSystem::LockCampus(ECampus campus, bool locked)
{
	FCampusStruct& c = GetCampusStruct_ECampus(campus);
	c.bIsLocked = locked;
}

bool UShieldSubSystem::CampusIsLocked(ECampus campus)
{
	FCampusStruct& c = GetCampusStruct_ECampus(campus);
	return c.bIsLocked;
}

EPasswordStrength UShieldSubSystem::GetPasswordStrength_Enum(ECampus campus)
{
	FCampusStruct campus_data = GetCampusStruct_ECampus(campus);
	return UPassword_Widget::GetPasswordStrength(campus_data.CurrentPassword);
}

TArray<ECampus> UShieldSubSystem::GetShieldedCampuses()
{
	TArray<ECampus> shielded;
	for(ECampus campus : TEnumRange<ECampus>())
	{
		if(!GetCampusStruct_ECampus(campus).bIsShieldDestroyed)
		{
			shielded.Add(campus);
		}
	}

	return shielded;
}

void UShieldSubSystem::ResetNodeTimer(ECampus campus)
{
	
	FCampusStruct& c = GetCampusStruct_ECampus(campus);
	c.Percentage = 1;
	c.bIsLowDelegateSent = false;
	c.bIsShieldDestroyed = false;
	UpdateCampusSlider(campus);
	OnTimerResetDelegate.Broadcast(campus);
}

void UShieldSubSystem::ResetHealth(ECampus campus, float rate)
{

	FCampusStruct& c = GetCampusStruct_ECampus(campus);
	c.MaxHealthPoints = rate;
	c.CurrentHealthPoints = rate;
	ResetNodeTimer(campus);
}

void UShieldSubSystem::OnCampusNodeEnd(ECampus campus)
{
	//broadcast to other function and UI
	OnTimerEndDelegate.Broadcast(campus);

	FCampusStruct& Cstruct = GetCampusStruct_ECampus(campus);

	Cstruct.Percentage = 0;
	Cstruct.bIsShieldDestroyed = true;
	
}

EAttackType UShieldSubSystem::ApplyDamageOnCampus(ECampus campus, float damage)
{
	UGI_Cybersecurity* gi = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
	FCampusStruct& c = GetCampusStruct_ECampus(campus);

	if(c.bIsShutdown) return EAttackType::None;

	if(!c.bIsShieldDestroyed)
	{
		c.CurrentHealthPoints -= damage*gi->DifficultyModifier;

		UpdateCampusSlider(campus);
		return EAttackType::Shield;
	}
	
	DataBaseHealth -= (damage/30)*gi->DifficultyModifier;
	OnDatabaseHealthChangedDelegate.Broadcast(DataBaseHealth);

	// loosing game -> need to create a function to reset all sub system
	if(DataBaseHealth <= 0)
	{
		
		if(!gi->bIsplayerDead)
		{
			gi->bIsplayerDead = true;
			// loosing game, will need some kind of transition
			UGameplayStatics::OpenLevel(this, "LVL_EndScreen");
		}

	}

	return EAttackType::Node;

}

void UShieldSubSystem::ApplyDamageToHealth(float damage)
{
	DataBaseHealth -= damage;
	OnDatabaseHealthChangedDelegate.Broadcast(DataBaseHealth);
}

void UShieldSubSystem::PopShield(ECampus target)
{
	FCampusStruct& c = GetCampusStruct_ECampus(target);
	if(c.CurrentHealthPoints > 0)
	{
		c.CurrentHealthPoints = 0;
		UpdateCampusSlider(target);
	}
}

ECampus UShieldSubSystem::GetAvailableTarget()
{
	TArray<ECampus> PossibleTargets = GetServerStates(false);

	if(PossibleTargets.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, PossibleTargets.Num()-1);
		return PossibleTargets[RandomIndex];
	}
	return ECampus::None;
}

TArray<ECampus> UShieldSubSystem::GetServerStates(bool bReturnDownServer)
{
	TArray<ECampus> ServerStates;
	if(NADMTL.bIsShutdown == bReturnDownServer)
	{
		ServerStates.Add(ECampus::NAD_MTL);
	}
	if(NADSBK.bIsShutdown == bReturnDownServer)
	{
		ServerStates.Add(ECampus::NAD_SBK);
	}
	if(UQAC.bIsShutdown == bReturnDownServer)
	{
		ServerStates.Add(ECampus::UQAC);
	}
	
	return ServerStates;
}

bool UShieldSubSystem::ShutdownNode(ECampus campus)
{
	FCampusStruct& c = GetCampusStruct_ECampus(campus);

	// if the server is not already shutdown 
	if(c.bIsShutdown == false)
	{
		c.bIsShutdown = true;
		// delegate called when the timer is finished
		FTimerDelegate ShutdownTimerDelegate;
		ShutdownTimerDelegate.BindUFunction(this, FName("PowerNode"), campus);
		// Countdown until the power is brought back online
		GetWorld()->GetTimerManager().SetTimer(ShutdownTimerHandles[CampusToIndex(campus)], ShutdownTimerDelegate, 60, false);
		GetWorld()->GetTimerManager().SetTimer(ShutdownScoreTimerHandles[CampusToIndex(campus)], this, &UShieldSubSystem::ShutdownScoreUpdate, 1, true);
		
		// have to call it once before starting the timer
		ShutdownScoreUpdate();

		
		OnNodeShutdownDelegate.Broadcast(campus);
		return  true;
	}

	return false;
}

void UShieldSubSystem::ShutdownScoreUpdate()
{
	UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
	UScoreSubSystem* ScoreSubsystem = gameInstancePtr->GetSubsystem<UScoreSubSystem>();

	ScoreSubsystem->ModifyScoreWithType(-ShutdownScoreCostPerSecond, EScoreType::SHIELD);
}

bool UShieldSubSystem::PowerNode(ECampus campus)
{
	//clear timer
	GetWorld()->GetTimerManager().ClearTimer(ShutdownTimerHandles[CampusToIndex(campus)]);
	GetWorld()->GetTimerManager().ClearTimer(ShutdownScoreTimerHandles[CampusToIndex(campus)]);
	// reset ui
	OnShutdownSliderValueChangedDelegate.Broadcast(campus, 1);
	
	FCampusStruct& c = GetCampusStruct_ECampus(campus);

	// if the server is not already shutdown 
	if(c.bIsShutdown == true)
	{
		c.bIsShutdown = false;
		OnNodePowerOnDelegate.Broadcast(campus);
		UE_LOG(LogTemp, Log, TEXT("Campus %hhd back online"), campus);
		return  true;
	}

	return false;
}

void UShieldSubSystem::UpdateCampusTargeting(ECampus campus, bool bIsTargeting)
{
	int32& Count = GetCampusStruct_ECampus(campus).AttackCounter;

	if (bIsTargeting)
	{
		// If this is the first target, fire the delegate
		if (Count == 0)
		{
			OnAttackedStateChangedDelegate.Broadcast(campus, true);
		}
		Count++;
	}
	else
	{
		if (Count > 0)
		{
			Count--;
			// If no more attackers, fire the delegate
			if (Count == 0)
			{
				OnAttackedStateChangedDelegate.Broadcast(campus, false);
			}
		}
	}
}

bool UShieldSubSystem::ComparePasswordToOld(const FString& Password)
{
	if(oldPasswords.Contains(Password))
	{
		return false;
	}

	return true;
}

bool UShieldSubSystem::ComparePasswordToLast(const FString& Password, ECampus campus)
{
	return GetCampusStruct_ECampus(campus).CurrentPassword.IsEmpty() || GetCampusStruct_ECampus(campus).CurrentPassword == Password;
}

bool UShieldSubSystem::CheckIfFirstPasswordsAreAllSet()
{
	TArray<FCampusStruct> campuses = GetCampusStructs();

	for (auto campus : campuses)
	{
		if(campus.CurrentPassword.IsEmpty())
		{
			return false;
		}
	}

	return true;
	
}

