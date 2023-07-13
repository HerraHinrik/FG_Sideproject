// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OnHitComponentBase.generated.h"

/**
 * Component that goes on projectiles to correctly calculate damage on hit.
 * 
 * WORK IN PROGRESS
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGICSAND_API UOnHitComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOnHitComponentBase();
protected:

	float BaseDamage;

	AController* Instigator;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ProcessOnHit(AActor* InActor, FVector HitLocation, FVector ImpactNormal);
};
