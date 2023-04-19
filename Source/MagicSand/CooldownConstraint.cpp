// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownConstraint.h"

UCooldownConstraint::UCooldownConstraint()
{
}

bool UCooldownConstraint::Evaluate() const
{
	return TimeElapsed >= Cooldown;
}

void UCooldownConstraint::ProcessFire()
{
	TimeElapsed = 0;
	bUsesTick = true;
}

void UCooldownConstraint::ResponseFunction()
{
	TimeElapsed = HUGE_VALF;
	bUsesTick = false;
}

void UCooldownConstraint::Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bUsesTick) TimeElapsed += DeltaTime;
}

void UCooldownConstraint::SetCooldown(float Value)
{
	Cooldown = Value;
}


