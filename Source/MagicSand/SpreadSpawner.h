// Fill out your copyright notice in the Description page of Project Settings.

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
	int BulletAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HalfAngle;

public:
	
	virtual TArray<AActor*> SpawnProjectiles_Implementation(FVector Location, FRotator Rotation) override;
};
