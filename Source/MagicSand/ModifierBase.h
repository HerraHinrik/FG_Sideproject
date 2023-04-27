// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstraintBase.h"
#include "ModifierBase.generated.h"

/**
 * Modifiers are UObjects that add/remove or tweak values of components of fired projectiles. 
 * 
 * They also support constraints that affect whether or not the modifier performs a change. 
 * Using constraints with a modifier requires a blueprint implementation of "Add Constraint"
 * and "Clear Constraints". For custom modifier c++ classes you must define a new
 * BP that implements these events for you.
 */
UCLASS(Blueprintable)
class MAGICSAND_API UModifierBase : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<class UActorComponent> TargetComponent;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	TArray<UConstraintBase*> ConstraintArray;

protected:

	// Override to add/alter projectile components
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ProcessSingle(AActor* Projectile);
	virtual void ProcessSingle_Implementation(AActor* Projectile);

public:

	virtual void Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	// Not for overriding
	void ProcessProjectiles(TArray<AActor*> ProjectileArray);

	UFUNCTION(BlueprintImplementableEvent)
	void AddConstraint(TSubclassOf<UConstraintBase> Constraint);

	UFUNCTION(BlueprintImplementableEvent)
	void ClearConstraints();
};
