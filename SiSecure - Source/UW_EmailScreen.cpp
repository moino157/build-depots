// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_EmailScreen.h"
#include "ScoreSubSystem.h"
#include "GI_Cybersecurity.h"
#include "NPCSubSystem.h"
#include "ShieldSubSystem.h"


void UUW_EmailScreen::OnScamButtonClicked()
{
    CheckEmail(false);
}

void UUW_EmailScreen::OnVerifiedButtonClicked()
{
    CheckEmail(true);
}

bool UUW_EmailScreen::OnHyperlinkClicked()
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UScoreSubSystem* scoreSubSystem = gameInstancePtr->GetSubsystem<UScoreSubSystem>();
    
    if (gameInstancePtr)
    {
        TArray<FComplexEmailStruct>& CurrentMailBox = gameInstancePtr->GetCurrentMailbox();

        if (CurrentMailBox.Num() > 0)
        {
            FComplexEmailStruct& CurrentEmail = CurrentMailBox[CurrentEmailIndex];

            CurrentEmail.bIsLinkedClicked = true;

            if (CurrentEmail.bIsARealEmail)
            {
                //add score
                scoreSubSystem->ModifyScoreWithType(50, EScoreType::EMAIL);
                return true;
            }
            else
            {
                //remove score
                scoreSubSystem->ModifyScoreWithType(-50, EScoreType::EMAIL);
                OnNotificationSentDelegate.Broadcast(AntivirusNotification);
                return false;
            }
        }

        return true;
    }

    return true;

}

ECampus UUW_EmailScreen::GetComputerLockScreen()
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());

    ECampus campus = gameInstancePtr->CurrentCampus;

    return campus;
}

void UUW_EmailScreen::CheckEmail(bool bClickedScam)
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UNPCSubSystem* NPCSubsystem = gameInstancePtr->GetSubsystem<UNPCSubSystem>();
    if (gameInstancePtr)
    {
        UScoreSubSystem* scoreSubSystem = gameInstancePtr->GetSubsystem<UScoreSubSystem>();
        TArray<FComplexEmailStruct>& CurrentMailBox = gameInstancePtr->GetCurrentMailbox();

        if (CurrentMailBox.Num() > 0)
        {
            FComplexEmailStruct CurrentEmail = CurrentMailBox[CurrentEmailIndex];
            CurrentMailBox.RemoveAt(CurrentEmailIndex);
            
            if ((bClickedScam && CurrentEmail.bIsARealEmail) || (!bClickedScam && !CurrentEmail.bIsARealEmail))
            {
                OnAnswerDelegate.Broadcast(true);

                //ignored link
                if (!bIsHyperlinkedClicked && CurrentEmail.bIsARealEmail)
                {
                    //remove boss negative notification but still no sure if we really have to
                    //OnNotificationSentDelegate.Broadcast(BosslinkIgnoredNotification);
                    scoreSubSystem->ModifyScoreWithType(-25, EScoreType::EMAIL);
                }
                else
                {


                    //adding score
                    scoreSubSystem->ModifyScoreWithType(30, EScoreType::EMAIL);


                    //remove boss positive notification but still no sure if we really have to
                    /*
                    gameInstancePtr->GoodEmailCount++;
                    if (gameInstancePtr->GoodEmailCount >= gameInstancePtr->NumberOfEmailBeforeBossCheer)
                    {
                        OnNotificationSentDelegate.Broadcast(BossPositiveNotification);
                        gameInstancePtr->GoodEmailCount = 0;
                    }
                    */
                }
            }
            else
            {
                OnAnswerDelegate.Broadcast(false);

                //remove boss negative notification but still no sure if we really have to
                //gameInstancePtr->BadEmailCount++;
                
                /*
                if (gameInstancePtr->BadEmailCount >= gameInstancePtr->NumberOfEmailBeforeBossWarning)
                {
                    OnNotificationSentDelegate.Broadcast(BossNegativeNotification);
                    gameInstancePtr->BadEmailCount = 0;
                }
                */

                //if the player acctepted a scam email -> fire anti virus notification
                if (!CurrentEmail.bIsARealEmail)
                {
                    OnNotificationSentDelegate.Broadcast(AntivirusNotification);

                    // Roll Chance of spawning a monster
                    NPCSubsystem->RollMonsterSpawn(
                        gameInstancePtr->CurrentCampus,
                        EMonsterType::BruteForce
                    );

                    //should we remove even more score if player accepted a scam email?
                    scoreSubSystem->ModifyScoreWithType(-15, EScoreType::EMAIL);
                }
                else
                {
                    //removing score
                    scoreSubSystem->ModifyScoreWithType(-15, EScoreType::EMAIL);
                }
            }

            bIsHyperlinkedClicked = false;
        }
    }
}
