#include "Password_Widget.h"
#include "ScoreSubSystem.h"
#include "GI_Cybersecurity.h"
#include "ShieldSubSystem.h"

UPassword_Widget::UPassword_Widget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer){ StrongNessCurrentPassword = EPasswordStrength::Fort; }

TArray<bool> UPassword_Widget::GetPasswordStrengthFlags(const FString& Password)
{
    int32 Length = Password.Len();
    int32 tqt = 0;
    bool bHasLower = false, bHasUpper = false;
    bool bHasDigit = false, bHasSpecialChar = false;
    bool bGreaterThan8 = Length >= 8;
    FString NormalChars = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");

    for (int32 i = 0; i < Length; i++)
    {
        if (!bHasLower) bHasLower = FChar::IsLower(Password[i]);
        if (!bHasUpper) bHasUpper = FChar::IsUpper(Password[i]);
        if (!bHasDigit) bHasDigit = FChar::IsDigit(Password[i]);
        if (!bHasSpecialChar) bHasSpecialChar = !NormalChars.FindChar(Password[i], tqt);
    }

    return {bHasLower, bHasUpper, bHasSpecialChar, bHasDigit, bGreaterThan8};
}

EPasswordStrength UPassword_Widget::GetPasswordStrength(const FString& Password)
{
    TArray<bool> flags = GetPasswordStrengthFlags(Password);
    int32 nbFlagValid = 0;
    for (bool flag : flags)
    {
        if (flag) nbFlagValid += 1;
    }

    if (nbFlagValid == 5) return EPasswordStrength::Fort;
    if (nbFlagValid < 5 && nbFlagValid > 3) return EPasswordStrength::Moyen;
    else return EPasswordStrength::Faible;
}

bool UPassword_Widget::OnResetPassword(const FString& Password)
{
    // add old password inside the shield sub system for persistent data
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;
    
    
    if (shieldSubSystem->oldPasswords.Contains(Password)) return false;

    AddNewPassword(Password);

    shieldSubSystem->ResetHealth(campus, NbOfMaxHealth);
    
    return true;
}

bool UPassword_Widget::SetNewPassword(const FString& Password)
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();
    ECampus campus = gameInstancePtr->CurrentCampus;

    if (shieldSubSystem->oldPasswords.Contains(Password)) return false;

    AddNewPassword(Password);
    shieldSubSystem->ResetHealth(campus, NbOfMaxHealth);
    return true;
}

bool UPassword_Widget::HasCommitedCorrectPassword(const FString& Password)
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;

    for (const FString& password : shieldSubSystem->oldPasswords)
    {
        UE_LOG(LogTemp, Log, TEXT("Password: %s"), *password);
    }

    return shieldSubSystem->oldPasswords.IsEmpty() || shieldSubSystem->GetCampusStruct_ECampus(campus).CurrentPassword == Password;
}

bool UPassword_Widget::FirstPassword()
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;

    FString CurrentPassword = shieldSubSystem->GetCampusStruct_ECampus(campus).CurrentPassword;

    return CurrentPassword.IsEmpty();
}

FText UPassword_Widget::AddText(FText text1, FText text2) const
{
    return FText::Format(NSLOCTEXT("MessageMotDePasse", "ConcatenatedText", "{0}{1}"), text1, text2);
}

void UPassword_Widget::ForgetPassword()
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;
    shieldSubSystem->GetCampusStruct_ECampus(campus).CurrentPassword = "";
    
    
}

void UPassword_Widget::UpdateScore(int score)
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UScoreSubSystem* scoreSubSystem = gameInstancePtr->GetSubsystem<UScoreSubSystem>();

    scoreSubSystem->ModifyScoreWithType(-score, EScoreType::PASSWORD);
    
}

FText UPassword_Widget::GetMissingMsg(FText textFaible, FText text2Moyen) const
{
    FText result;
    if (StrongNessCurrentPassword == EPasswordStrength::Fort) result = FText::GetEmpty();
    else if (StrongNessCurrentPassword == EPasswordStrength::Moyen) result = text2Moyen;
    else if (StrongNessCurrentPassword == EPasswordStrength::Faible) result = textFaible;

    return result;
}

ECampus UPassword_Widget::GetCampus() const
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());

    ECampus campus = gameInstancePtr->CurrentCampus;
    return campus;
}

void UPassword_Widget::LockCampus(bool locked)
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;

    shieldSubSystem->LockCampus(campus, locked);
}

bool UPassword_Widget::CampusIsLocked() const
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;
    return shieldSubSystem->CampusIsLocked(campus);
}

void UPassword_Widget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPassword_Widget::ResetTimer()
{
    if (StrongNessCurrentPassword == EPasswordStrength::Fort) NbOfMaxHealth = 120;
    else if (StrongNessCurrentPassword == EPasswordStrength::Moyen) NbOfMaxHealth = 60;
    else if (StrongNessCurrentPassword == EPasswordStrength::Faible) NbOfMaxHealth = 30;
    else return;

    Timer = NbOfMaxHealth;
    SetTimerBar(1.0f);
    IsServerHack = false;
}

void UPassword_Widget::SetTimerBar(float percent)
{
    if (TimerBar_2)
    {
        TimerBar_2->SetPercent(percent);
    }
}

void UPassword_Widget::UpdateTimer()
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();

    // get remaining time
    //Timer = GetWorld()->GetTimerManager().GetTimerRemaining(shieldSubSystem->GetCampusTimerHandle(campusName)); 
    
    
    if (Timer > 0)
    {
        Timer -= tick;
        SetTimerBar(Timer / NbOfMaxHealth);
        if (Timer < 20 && !OnTimerLowIsActivate)
        {
            OnTimerLow.Broadcast(); // Déclenche l'événement lorsque le timer est low
            OnTimerLowIsActivate = true;
        }
    }
    else
    {
        // clear subsystem timer
        //GetWorld()->GetTimerManager().ClearTimer(shieldSubSystem->GetCampusTimerHandle(campusName));
        
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        OnTimerExpired.Broadcast(); // Déclenche l'événement lorsque le timer atteint zéro
        IsServerHack = true;
    }
}

void UPassword_Widget::AddNewPassword(const FString& Password)
{
    UGI_Cybersecurity* gameInstancePtr = Cast<UGI_Cybersecurity>(GetWorld()->GetGameInstance());
    UShieldSubSystem* shieldSubSystem = gameInstancePtr->GetSubsystem<UShieldSubSystem>();
    UScoreSubSystem* scoreSubSystem = gameInstancePtr->GetSubsystem<UScoreSubSystem>();

    ECampus campus = gameInstancePtr->CurrentCampus;

    shieldSubSystem->oldPasswords.Add(Password);
    shieldSubSystem->GetCampusStruct_ECampus(campus).CurrentPassword = Password;
    
    switch (GetPasswordStrength(Password))
    {
        case EPasswordStrength::Faible:
            NbOfMaxHealth = 2;
            scoreSubSystem->ModifyScoreWithType(-10, EScoreType::PASSWORD);
            break;
        case EPasswordStrength::Moyen:
            NbOfMaxHealth = 5;
            scoreSubSystem->ModifyScoreWithType(10, EScoreType::PASSWORD);
            break;
        case EPasswordStrength::Fort:
            NbOfMaxHealth = 10;
            scoreSubSystem->ModifyScoreWithType(20, EScoreType::PASSWORD);
            break;
    }
}
