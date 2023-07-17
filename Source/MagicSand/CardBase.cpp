// Fill out your copyright notice in the Description page of Project Settings.

#include "CardBase.h"
#include "FirstPersonViewCharacter.h"


void UCardBase::ApplyWeaponModifier(AFirstPersonViewCharacter* TargetCharacter)
{
	if (!IsValid(TargetCharacter)) return;

	UGunComponent* Weapon = TargetCharacter->GetWeaponComponenet();

	if (!IsValid(Weapon)) return;

	Weapon->ApplyModifier(WeaponModifier);

}

void UCardBase::PlayCard(AFirstPersonViewCharacter* TargetCharacter)
{
	ApplyWeaponModifier(TargetCharacter);
	//ApplyPlayerModifier(TargetCharacter);
}

int UCardBase::GetID()
{
	return CardID;
}