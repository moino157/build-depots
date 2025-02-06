#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Internationalization/StringTableRegistry.h"
#include "Components/ProgressBar.h"
#include "Logging/LogMacros.h"
#include "Password_Widget.generated.h"

enum class EPasswordStrength : uint8;
class UScoreSubSystem;

enum class ECampus : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerExpired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerLow);

UCLASS()
class CYBERSECURITE_API UPassword_Widget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPassword_Widget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Campus")
    ECampus campusName;

    UFUNCTION(BlueprintCallable, Category = "Password")
    static TArray<bool> GetPasswordStrengthFlags(const FString& Password); // Utiliser FText au lieu de FString

    UFUNCTION(BlueprintCallable, Category = "Password")
    static EPasswordStrength GetPasswordStrength(const FString& Password);

    UFUNCTION(BlueprintCallable, Category = "Password")
    bool OnResetPassword(const FString& Password);

    UFUNCTION(BlueprintCallable, Category = "Password")
    bool SetNewPassword(const FString& Password);

    UFUNCTION(BlueprintCallable, Category = "Password")
    bool HasCommitedCorrectPassword(const FString& Password);

    UFUNCTION(BlueprintCallable, Category = "Password")
    bool FirstPassword();

    UFUNCTION(BlueprintCallable, Category = "Password")
    FText AddText(FText text1, FText text2) const;

    UFUNCTION(BlueprintCallable, Category = "Password")
    void ForgetPassword();

    UFUNCTION(BlueprintCallable, Category = "Password")
    void UpdateScore(int score);

    UFUNCTION(BlueprintCallable, Category = "Password")
    FText GetMissingMsg(FText textFaible, FText text2Moyen) const;

    UFUNCTION(BlueprintCallable, Category = "Password")
    ECampus GetCampus() const;


    UPROPERTY(BlueprintAssignable, Category = "Password")
    FOnTimerExpired OnTimerExpired;

    UPROPERTY(BlueprintAssignable, Category = "Password")
    FOnTimerLow OnTimerLow;
    
    UPROPERTY(BlueprintReadWrite, Category = "Password")
    class UProgressBar* TimerBar_2;

    UPROPERTY(BlueprintReadOnly, Category = "Password")
    bool IsServerHack;

    UPROPERTY(BlueprintReadWrite, Category = "Password")
    TArray<FString> oldPasswords;

    UPROPERTY(BlueprintReadWrite, Category = "Password")
    FString currentPassword;

    UFUNCTION(BlueprintCallable, Category = "Password")
    void LockCampus(bool isLocked);

    UFUNCTION(BlueprintCallable, Category = "Password")
    bool CampusIsLocked() const;

protected:
    virtual void NativeConstruct() override;

public:
    EPasswordStrength StrongNessCurrentPassword;

    UPROPERTY(BlueprintReadWrite, Category = "Password")
    float Timer = 120;

private:

    float NbOfMaxHealth = 120;

    const float tick = 0.1f;

    void ResetTimer();
    void SetTimerBar(float percent);

    FTimerHandle TimerHandle;

    void UpdateTimer();

    bool OnTimerLowIsActivate = false;

    void AddNewPassword(const FString& Password);
};