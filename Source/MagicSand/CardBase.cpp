// Fill out your copyright notice in the Description page of Project Settings.

#include "CardBase.h"
#include "FirstPersonViewCharacter.h"


void UCardBase::ApplyWeaponModifier(AFirstPersonViewCharacter* TargetCharacter)
{
	if (!IsValid(TargetCharacter) || !IsValid(WeaponModifier)) return;

	UGunComponent* Weapon = TargetCharacter->GetWeaponComponenet();

	if (!IsValid(Weapon)) return;

	Weapon->ApplyModifier(WeaponModifier);

}
void UCardBase::ApplyPlayerModifier(AFirstPersonViewCharacter* TargetCharacter)
{
	if (!IsValid(TargetCharacter) || !IsValid(PlayerModifier)) return;

	UPlayerModifierComponent* PlayerStats = TargetCharacter->GetPlayerModifierComponent();

	if (!IsValid(PlayerStats)) return;

	PlayerStats->ApplyPlayerModifier(PlayerModifier);
}

void UCardBase::PlayCard(AFirstPersonViewCharacter* TargetCharacter)
{
	ApplyWeaponModifier(TargetCharacter);
	ApplyPlayerModifier(TargetCharacter);
}

int32 UCardBase::GetID()
{
	return CardID;
}


