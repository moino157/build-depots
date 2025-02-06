#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UTimerSystem.generated.h"

UCLASS()
class CYBERSECURITE_API UTimerSystem : public UObject
{
    GENERATED_BODY()

public:
    UTimerSystem();

    const float tick = 0.1f;

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StartTimer();

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StopTimer();

    UPROPERTY(BlueprintReadOnly, Category = "Password")
    float ElapsedTime;

protected:
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    float TickInterval;

    UFUNCTION()
    void UpdateTimer();
};
