// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstraintBase.h"
#include "CooldownConstraint.generated.h"

/**
 * 
 */
UCLASS()
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
	virtual bool Evaluate() const override;

	virtual void ProcessFire() override;

	virtual void OnReload() override;
	
	virtual void Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End Inherited

	UFUNCTION(BlueprintCallable)
	void SetCooldown(float Value);

};
