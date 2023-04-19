// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstraintBase.h"
#include "ModifierBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MAGICSAND_API UModifierBase : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<class UActorComponent> TargetComponent;

protected:

	// Override to add/alter projectile components
	virtual void ProcessSingle(AActor* Projectile);

public:


	virtual void Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	// Do not override, will lose constraint support
	void ProcessProjectiles(TArray<AActor*> ProjectileArray);
};
