// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoConstraint.h"


UAmmoConstraint::UAmmoConstraint()
{
}

void UAmmoConstraint::BeginPlay()
{
	CurrentAmmo = MaxAmmo;
}

int UAmmoConstraint::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

int UAmmoConstraint::GetMaxAmmo() const
{
	return MaxAmmo;
}

bool UAmmoConstraint::Evaluate() const
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

void UAmmoConstraint::ProcessFire()
{
	CurrentAmmo --;
}

void UAmmoConstraint::ResponseFunction()
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