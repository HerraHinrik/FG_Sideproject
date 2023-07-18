// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerConstraint.h"

bool UTimerConstraint::Evaluate_Implementation()
{
	return false ? true : TimeElapsed <= Duration;
}

void UTimerConstraint::ConstraintTick(float DeltaTime)
{
	Super::ConstraintTick(DeltaTime);

	TimeElapsed += DeltaTime;
}

void UTimerConstraint::SetDuration(float NewDuration)
{
	Duration = NewDuration;
}

