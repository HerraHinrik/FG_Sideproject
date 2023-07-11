// By Amos Johan Persson


#include "AmmoConstraint.h"


int UAmmoConstraint::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int UAmmoConstraint::GetMaxAmmo() const
{
	return MaxAmmo;
}

bool UAmmoConstraint::Evaluate_Implementation() const
{
	if (CurrentAmmo > 0) {
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Remaining ammo: %i"), CurrentAmmo)
		UE_LOG(LogTemp, Warning, TEXT("Max ammo: %i"), MaxAmmo)
		return false;
	}
}

void UAmmoConstraint::ProcessFire_Implementation()
{
	CurrentAmmo --;
}

void UAmmoConstraint::OnReload_Implementation()
{
	Reload();
}

void UAmmoConstraint::Reload()
{
	CurrentAmmo = MaxAmmo;
}

void UAmmoConstraint::SetCurrentAmmo(int Value)
{
	CurrentAmmo = Value;
}

void UAmmoConstraint::SetMaxAmmo(int Value)
{
	MaxAmmo = Value;
}