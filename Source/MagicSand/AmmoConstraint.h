// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstraintBase.h"
#include "AmmoConstraint.generated.h"


/**
 * Tracks ammo and says no when the player is out of ammo. 
 */
UCLASS(Blueprintable)
class UAmmoConstraint : public UConstraintBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxAmmo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int CurrentAmmo;

public:

public:

	// Inherited
	virtual bool Evaluate_Implementation() const;

	virtual void ProcessFire_Implementation() override;

	virtual void OnReload_Implementation() override;
	// End Inherited

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable)
	int GetMaxAmmo() const;


	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(int Value);

	UFUNCTION(BlueprintCallable)
	void SetMaxAmmo(int Value);

};
