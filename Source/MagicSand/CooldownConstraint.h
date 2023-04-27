// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstraintBase.h"
#include "CooldownConstraint.generated.h"

/**
 * Keeps internal track of cooldown using GunComponent's tick. Fire-rate is controlled with this. 
 */
UCLASS(Blueprintable)
class UCooldownConstraint : public UConstraintBase
{
	GENERATED_BODY()

	UCooldownConstraint();

protected:

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Cooldown;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float TimeElapsed = HUGE_VALF;

public:
	// Inherited
	bool Evaluate_Implementation() const override;

	virtual void ProcessFire_Implementation() override;

	virtual void OnReload_Implementation() override;
	
	virtual void Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End Inherited

	UFUNCTION(BlueprintCallable)
	void SetCooldown(float Value);

};
