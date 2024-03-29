// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "SpawnerBase.h"
#include "SpreadSpawner.generated.h"

/**
 * Shotgun-type spawner. 
 */
UCLASS()
class MAGICSAND_API USpreadSpawner : public USpawnerBase
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	FVector GetDirectionFromCone();

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 BulletAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HalfAngle;

public:
	
	virtual TArray<AProjectileBase*> SpawnProjectiles_Implementation(FVector Location, FRotator Rotation, int32 RawDamageBoost, float DamagePercentageBoost) override;
};
