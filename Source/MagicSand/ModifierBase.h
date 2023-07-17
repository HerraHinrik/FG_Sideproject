// By Amos Johan Persson
#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "UObject/NoExportTypes.h"
#include "ConstraintBase.h"
#include "ProjectileBase.h"
#include "TimerConstraint.h"
#include "ModifierBase.generated.h"

/**
 * Modifiers are UObjects that duplicate/delete/edit/change projectiles. 
 * 
 * Override ProcessSingle_Implementation to make custom behavior. Remember modifiers receive 
 * one projectile and return an array of projectiles (may be empty) for the next modifier in 
 * the chain -- this is done on a per-projectile basis. 
 * 
 * They also support constraints that affect whether or not the modifier performs a change.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGunModifierDelegate, UModifierBase*, ModifierObject);

UCLASS(Blueprintable)
class MAGICSAND_API UModifierBase : public UObject
{
	GENERATED_BODY()

public:
	UModifierBase();

private:
	UPROPERTY()
	bool bUsesTick = false;

	// Track The last frame number we were ticked.
	uint32 LastTickFrame = INDEX_NONE;

private:
	bool CheckConstaints();
	void UpdateConstraints();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float LifetimeDuration = 10;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	TArray<UConstraintBase*> ConstraintArray;

protected:

	// Override to add/alter projectile components
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<AProjectileBase*> ProcessSingle(AProjectileBase* Projectile);
	virtual TArray<AProjectileBase*> ProcessSingle_Implementation(AProjectileBase* Projectile);

public:
	virtual void ModifierTick(float DeltaTime);

	bool CanTick() const;

	// Not for overriding
	TArray<AProjectileBase*> ProcessProjectiles(TArray<AProjectileBase*> ProjectileArray);

	UFUNCTION(BlueprintCallable)
	void AddConstraint(TSubclassOf<UConstraintBase> Constraint);

	UFUNCTION(BlueprintCallable)
	void ClearConstraints();
};
