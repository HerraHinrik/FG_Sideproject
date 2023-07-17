// By Amos Johan Persson


#include "CooldownConstraint.h"

bool UCooldownConstraint::Evaluate_Implementation()
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

void UCooldownConstraint::ConstraintTick(float DeltaTime)
{
	if (!CanTick()) return;

	Super::ConstraintTick(DeltaTime);

	TimeElapsed += DeltaTime;
}

void UCooldownConstraint::SetCooldown(float Value)
{
	Cooldown = Value;
}


