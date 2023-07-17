// By Amos Johan Persson


#include "CooldownConstraint.h"

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
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
}

void UCooldownConstraint::SetCooldown(float Value)
{
	Cooldown = Value;
}


