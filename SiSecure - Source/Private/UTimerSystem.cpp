#include "UTimerSystem.h"
#include "TimerManager.h"

UTimerSystem::UTimerSystem()
{
    ElapsedTime = 0.0f;
    TickInterval = 0.0f;
}

void UTimerSystem::StartTimer()
{
    ElapsedTime = 0.0f;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerSystem::UpdateTimer, tick, true);
}

void UTimerSystem::StopTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UTimerSystem::UpdateTimer()
{
    ElapsedTime += tick;
}
