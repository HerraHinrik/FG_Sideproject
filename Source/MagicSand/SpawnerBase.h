// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
	TSubclassOf<class AActor> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ESpawnActorCollisionHandlingMethod SpawnActorCollisionHandlingMethod;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

protected:

	AActor* SpawnActor(
		UClass * InClass, 
		const FVector* Location = (const FVector*)0,
		const FRotator* Rotation = (const FRotator*)0,
		FActorSpawnParameters SpawnParams = FActorSpawnParameters()
	);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnSingleProjectile(FVector Location, FRotator Rotation);

public:

	// Override this for custom spawning, 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AActor*> SpawnProjectiles(FVector Location, FRotator Rotation);
	virtual TArray<AActor*> SpawnProjectiles_Implementation(FVector Location, FRotator Rotation);
	
};
