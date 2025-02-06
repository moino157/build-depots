// 

#pragma once

#include "CoreMinimal.h"
#include "GunComponent.h"
#include "RifleComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSCIFI_API URifleComponent : public UGunComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URifleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	// Override Fire method
	//virtual void Fire() override;

	// Override Reload method
	virtual void Reload() override;
};
