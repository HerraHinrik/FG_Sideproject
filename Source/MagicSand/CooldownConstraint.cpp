// By Amos Johan Persson


#include "CooldownConstraint.h"

UCooldownConstraint::UCooldownConstraint()
{
}

bool UCooldownConstraint::Evaluate_Implementation() const
{
	return TimeElapsed >= Cooldown;
}

void UCooldownConstraint::ProcessFire_Implementation()
{
	TimeElapsed = 0;
	bUsesTick = true;
}

void UCooldownConstraint::OnReload_Implementation()
{
	TimeElapsed = HUGE_VALF;
	bUsesTick = false;
}

void UCooldownConstraint::Tick(float DeltaTime)
{
	TimeElapsed += DeltaTime;
}

void UCooldownConstraint::SetCooldown(float Value)
{
	Cooldown = Value;
}


