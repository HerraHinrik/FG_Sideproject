// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstraintBase.h"
#include "TimerConstraint.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGunTimerDelegate);

UCLASS()
class MAGICSAND_API UTimerConstraint : public UConstraintBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable);
	FGunTimerDelegate OnExpire;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Duration;

	float TimeElapsed = 0;

public:
	// Inherited
	bool Evaluate_Implementation() override;

	virtual void ConstraintTick(float DeltaTime) override;
	// End Inherited

	void SetDuration(float NewDuration);
};
