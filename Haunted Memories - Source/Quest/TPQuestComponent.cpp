// Fill out your copyright notice in the Description page of Project Settings.


#include "TPQuestComponent.h"

// Sets default values for this component's properties
UTpQuestComponent::UTpQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTpQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	QuestsValidated.Init(false,QuestDataAsset->Quests.Num());
	
	FQuestStruct quest = QuestDataAsset->Quests[currentQuestId];
	OnQuestUpdate.Broadcast(quest.QuestName,quest.Description);
	// ...
	
}

void UTpQuestComponent::UpdateQuest()
{
	if(QuestsValidated.Num() > 0){
		while(QuestsValidated[currentQuestId])
		{
			currentQuestId++;
		}
	}
	
	FQuestStruct quest = QuestDataAsset->Quests[currentQuestId];
	OnQuestUpdate.Broadcast(quest.QuestName,quest.Description);
}

void UTpQuestComponent::ValidateQuest(int questId)
{
	if(!QuestsValidated.IsValidIndex(questId))
	{
		if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("FAILED TO VALIDATE QUEST; QUEST ID IS INVALID"));}
		return;
	}
	QuestsValidated[questId] = true;
	UpdateQuest();
}
