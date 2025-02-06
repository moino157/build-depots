// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Cybersecurity.h"

#include "NPCSubSystem.h"
#include "ScoreSubSystem.h"
#include "Containers/Set.h"
#include "TimerManager.h"
#include "ShieldSubSystem.h"


void UGI_Cybersecurity::SetCurrentCampus(ECampus campus)
{
	CurrentCampus = campus;
}

void UGI_Cybersecurity::Init()
{

	Super::Init();
	
}

void UGI_Cybersecurity::ResetGame()
{
	//clear timers
	GetWorld()->GetTimerManager().ClearTimer(EmailGenerationTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(EmailAmountCheckTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DifficultyTimerHandle);

	// reset player variables
	bIsplayerDead = false;
	PlayerName = "";
	BadEmailCount = 0;
	GoodEmailCount = 0;
	DifficultyModifier = 0.35;

	//empty mail boxes 
	UQAC_Mailbox.Empty();
	NADMTL_Mailbox.Empty();
	NADSBK_Mailbox.Empty();

	
	// call other subsystems reset
	UNPCSubSystem* NpcSubSystem = GetSubsystem<UNPCSubSystem>();
	NpcSubSystem->ResetSystem();

	UScoreSubSystem* ScoreSubSystem = GetSubsystem<UScoreSubSystem>();
	ScoreSubSystem->ResetSystem();

	UShieldSubSystem* ShieldSubSystem = GetSubsystem<UShieldSubSystem>();
	ShieldSubSystem->ResetSystem();
	

	// start in easymode
	ChangeDifficulty(EasyGamemodeValues);

	CurrentCampus = ECampus::None;
}

void UGI_Cybersecurity::StartGame()
{
	// get next email generation time
	CurrentEmailGenerationInterval = PrePasswordEmailGenerationTime;

	GetWorld()->GetTimerManager().SetTimer(
		DifficultyTimerHandle,
		this, &UGI_Cybersecurity::IncreaseDifficulty,	// Increase by 5% difficulty
		60,	// Difficulty is increased each minute
		true
	);


	GetWorld()->GetTimerManager().SetTimer(
		EmailGenerationTimerHandle,
		this, &UGI_Cybersecurity::AddEmailtoAllMailbox,
		CurrentEmailGenerationInterval,
		false
	);
	GetWorld()->GetTimerManager().SetTimer(EmailAmountCheckTimerHandle, this, &UGI_Cybersecurity::CheckIfBoxIsOverfilled, filledMailboxWarningInterval, true);

	SpawnStartingEmail();

	// need to init subsystem after the player get for the first time in the holomap
	UNPCSubSystem* NpcSubSystem = GetSubsystem<UNPCSubSystem>();
	NpcSubSystem->StartSystem();

	UShieldSubSystem* ShieldSubSystem = GetSubsystem<UShieldSubSystem>();
	ShieldSubSystem->StartSystem();
}

void UGI_Cybersecurity::AddEmailtoAllMailbox()
{
	FComplexEmailStruct email;

	// generate email then send broadcast
	email = GenerateEmail();
	UQAC_Mailbox.Add(email);
	OnEmailCreatedDelegate.Broadcast(email, "UQAC");

	email = GenerateEmail();
	NADSBK_Mailbox.Add(GenerateEmail());
	OnEmailCreatedDelegate.Broadcast(email, "SBK");

	email = GenerateEmail();
	NADMTL_Mailbox.Add(GenerateEmail());
	OnEmailCreatedDelegate.Broadcast(email, "MTL");


	//clear the timer
	GetWorld()->GetTimerManager().ClearTimer(EmailGenerationTimerHandle);

	//calculate new interval
	UShieldSubSystem* ShieldSubSystem = GetSubsystem<UShieldSubSystem>();
	if(ShieldSubSystem->CheckIfFirstPasswordsAreAllSet())
	{
		CurrentEmailGenerationInterval = GetRandomTimerValueBasedOnWeight();
		UE_LOG(LogTemp, Log, TEXT("email interval is %f"), CurrentEmailGenerationInterval);
	}
	else
	{
		CurrentEmailGenerationInterval = PrePasswordEmailGenerationTime;
	}
	
	// insert the new timer value
	GetWorld()->GetTimerManager().SetTimer(EmailGenerationTimerHandle, this, &UGI_Cybersecurity::AddEmailtoAllMailbox, CurrentEmailGenerationInterval, false);
	OnCountDelegate.Broadcast();
	
}

void UGI_Cybersecurity::CheckIfBoxIsOverfilled()
{
	bool bIsOnBoxOverfilled = false;

	OverfilledNotification.AdditionalImage1 = nullptr;
	OverfilledNotification.AdditionalImage2 = nullptr;
	OverfilledNotification.AdditionalImage3 = nullptr;
	
	if (UQAC_Mailbox.Num() >= MailBoxOverfilledNotification)
	{
		bIsOnBoxOverfilled = true;
		OverfilledNotification.AdditionalImage1 = UqacLogo;
	}
	if(NADMTL_Mailbox.Num() >= MailBoxOverfilledNotification)
	{
		bIsOnBoxOverfilled = true;
		OverfilledNotification.AdditionalImage2 = MontrealLogo;
	}
	if (NADSBK_Mailbox.Num() >= MailBoxOverfilledNotification)
	{
		bIsOnBoxOverfilled = true;
		OverfilledNotification.AdditionalImage3 = SherbrookeLogo;
	}

	if (bIsOnBoxOverfilled)
	{
		OverfilledNotification.NotificationPrompt = "Attention tu dois essayer de ne pas entasser tes emails dans :";
		OnOverfilledMailboxDelegate.Broadcast(OverfilledNotification);
		
		UScoreSubSystem* scoreSubSystem = GetSubsystem<UScoreSubSystem>();

		scoreSubSystem->ModifyScoreWithType(-5, EScoreType::EMAIL);
		
	}
}

void UGI_Cybersecurity::ChangeDifficulty(FGamemodeDifficultyStruct difficulty)
{
	CurrentDifficulty = difficulty;
}

TArray<FComplexEmailStruct>& UGI_Cybersecurity::GetCurrentMailbox()
{
	FString CurrentMapName = GetWorld()->GetMapName();

	if (CurrentMapName.Contains("UQAC"))
	{
		return UQAC_Mailbox;
	}
	else if(CurrentMapName.Contains("MTL"))
	{
		return NADMTL_Mailbox;
	}

	return NADSBK_Mailbox;

}

void UGI_Cybersecurity::SpawnStartingEmail()
{
	for (int i = 0; i < StartingEmailAmount; i++)
	{
		UQAC_Mailbox.Add(GenerateEmail());
		NADSBK_Mailbox.Add(GenerateEmail());
		NADMTL_Mailbox.Add(GenerateEmail());
	}
}

FComplexEmailStruct UGI_Cybersecurity::GenerateEmail()
{

	int32 RandomValue = FMath::RandRange(0, 99);

	FComplexEmailStruct SelectedEmail;

	//TODO maybe add a priority for nodes with shield?
	//UShieldSubSystem* ShieldSubSystem = GetSubsystem<UShieldSubSystem>();

	//available node does nothing right now
	SelectedEmail.CC = PlayerName + ", " + GetRandomAvailableNode();

	// chance to get a normal email
	if (RandomValue < CurrentDifficulty.ImportantDatabasePercentage)
	{
		//need a place to enter player name
		SelectedEmail.Salutation = "Bonjour " + PlayerName + ",";
		SelectedEmail.SenderInformation = GetRandomRowFromDataTable<FSenderProfileStruct>(ImportantEmailDataSet.SenderProfileDataset);
		SelectedEmail.Content = GetRandomRowFromDataTable<FContentStruct>(ImportantEmailDataSet.ContentDataset);
		SelectedEmail.bIsARealEmail = true;
		
	}
	//the email is a scam
	else
	{
		SelectedEmail.bIsARealEmail = false;

		TSet<int32> UniqueNumbers;

		//check if there must be easy elements in the current difficulty then set them to easy
		//SetElementValueWithMin(SelectedEmail, CurrentDifficulty.EasyMinimumValues, EasyEmailDataSet, UniqueNumbers);

		//check if there must be medium elements in the current difficulty then set them to medium
		//SetElementValueWithMin(SelectedEmail, CurrentDifficulty.MediumMinimumValues, MediumEmailDataSet, UniqueNumbers);


		TArray<int32> ValidIndices = { 0, 1};
		// Iterate through all valid indices
		for (int32 Index : ValidIndices)
		{
			// Check if the index is missing from UniqueNumbers
			if (!UniqueNumbers.Contains(Index))
			{
				RandomValue = FMath::RandRange(0, 99);

				if (RandomValue < CurrentDifficulty.EasyDatabasePercentage)
				{
					SelectedEmail.Salutation = "Cher utilisateur, ";
					SetElementValueWithIndex(SelectedEmail, EasyEmailDataSet, Index);
				}
				else
				{
					SelectedEmail.Salutation = "Bonjour " + PlayerName + ",";
					SetElementValueWithIndex(SelectedEmail, MediumEmailDataSet, Index);
				}
			}
		}


	}


	return SelectedEmail;

}

int UGI_Cybersecurity::GetCampusMailBoxCount(ECampus campus)
{
	switch (campus)
	{
	case ECampus::UQAC:
		return UQAC_Mailbox.Num();
	case ECampus::NAD_MTL:
		return NADMTL_Mailbox.Num();
	case ECampus::NAD_SBK:
		return NADSBK_Mailbox.Num();
	default:
			return 0;
	}
}

void UGI_Cybersecurity::SetElementValueWithMin(FComplexEmailStruct& email, int MinValue, const FEmailDifficultyStruct& dataset, TSet<int32>& set)
{
	int32 LoopCounter = 0;


	// if some elements have to be easy then select random element
	while (set.Num() < MinValue)
	{
		int32 RandomNumber = FMath::RandRange(0, 3);

		const int32 PreviousSize = set.Num();
		set.Add(RandomNumber);

		// succesfully add a number
		if (set.Num() > PreviousSize)
		{
			switch (RandomNumber)
			{
			case 0:
				email.SenderInformation = GetRandomRowFromDataTable<FSenderProfileStruct>(dataset.SenderProfileDataset);
			case 1:
				email.Content = GetRandomRowFromDataTable<FContentStruct>(dataset.ContentDataset);
			default:
				break;
			}
		}

		LoopCounter++;

		if (LoopCounter > 100)
		{
			break;
		}
	}
}

void UGI_Cybersecurity::SetElementValueWithIndex(FComplexEmailStruct& email, const FEmailDifficultyStruct& dataset, int32 index)
{
	switch (index)
	{
	case 0:
		email.SenderInformation = GetRandomRowFromDataTable<FSenderProfileStruct>(dataset.SenderProfileDataset);
	case 1:
		email.Content = GetRandomRowFromDataTable<FContentStruct>(dataset.ContentDataset);
	default:
		break;
	}
}

float UGI_Cybersecurity::GetRandomTimerValueBasedOnWeight()
{
	const float DifficultyCoefficient = ((DifficultyModifier -1) * 4) + 1;
	// Calculate the total weight
	int32 TotalWeight = 0;
	for (const FTimerWeight& TimerWeight : EmailGenerationFrequencyCurve)
	{
		TotalWeight += TimerWeight.Weight;
	}

	// Select a random value based on the total weight
	int32 RandomValue = FMath::RandRange(0, TotalWeight - 1);
	for (const FTimerWeight& TimerWeight : EmailGenerationFrequencyCurve)
	{
		RandomValue -= TimerWeight.Weight;
		if (RandomValue < 0)
		{
			return TimerWeight.Time / DifficultyCoefficient;
		}
	}

	return 0.0f;
}

FString UGI_Cybersecurity::GetRandomAvailableNode()
{

	TArray<FString> HealthyNodes;

	// Check each node state and add the node name if it's healthy
	if (UqacNodeState == CampusNodeState::HEALTHY)
	{
		HealthyNodes.Add("UQAC");
	}
	if (NAD_SBKNodeState == CampusNodeState::HEALTHY)
	{
		HealthyNodes.Add("NAD Sherbrooke");
	}
	if (NAD_MTLNodeState == CampusNodeState::HEALTHY)
	{
		HealthyNodes.Add("NAD Montreal");
	}

	// TODO ---> If there are no healthy nodes game over
	if (HealthyNodes.Num() == 0)
	{
		return FString("No available healthy nodes");
	}

	// Randomly select a healthy node from the list
	int32 RandomIndex = FMath::RandRange(0, HealthyNodes.Num() - 1);
	return HealthyNodes[RandomIndex];
}

void UGI_Cybersecurity::IncreaseDifficulty()
{
	DifficultyModifier *= 1.5;
	UE_LOG(LogTemp, Log, TEXT("Difficulty multiplyer is now %f"), DifficultyModifier);
}

