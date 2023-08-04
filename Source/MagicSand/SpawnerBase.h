// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ProjectileBase.h"
#include "SpawnerBase.generated.h"


/**
 * Spawners represent a strategy for spawning actors. Intended use case is spawning projectiles when a gun is fired.
 * 
 * Spawners are agnostic towards what they spawn, they only carry logic for how many and how to space them out. Spawners
 * have no real dependencies and this class can be subclassed directly.
 * 
 * Function to override:
 *		- Spawn Projectiles <-- this is where placement logic happens
 */
UCLASS(Blueprintable)
class USpawnerBase : public UObject
{
	GENERATED_BODY()

public:
	USpawnerBase();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);


	UPROPERTY(BlueprintReadWrite)
	int32 TeamNumber;

protected:

	AProjectileBase* SpawnProjectile(
		TSubclassOf<AProjectileBase> InClass,
		const FVector* Location = (const FVector*)0,
		const FRotator* Rotation = (const FRotator*)0,
		FActorSpawnParameters SpawnParams = FActorSpawnParameters()
	);

	UFUNCTION(BlueprintCallable)
	AProjectileBase* SpawnSingleProjectile(FVector Location, FRotator Rotation, int32 RawDamageBoost, float DamagePercentageBoost);

public:

	// Override this for custom spawning
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AProjectileBase*> SpawnProjectiles(FVector Location, FRotator Rotation, int32 RawDamageBoost, float DamagePercentageBoost);
	virtual TArray<AProjectileBase*> SpawnProjectiles_Implementation(FVector Location, FRotator Rotation, int32 RawDamageBoost, float DamagePercentageBoost);

	UFUNCTION()
		void SetTeamNumber(int32 Team);
	
};
