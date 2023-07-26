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
	PlayerModifier = FPlayerModifier();
	FPlayerStatBlock Stats = FPlayerStatBlock();

	Stats.Armor = ArmorAdjustment;
	Stats.DamageFlat = RawDamageAdjustment;
	Stats.DamageMultiplier = DamageMultiplierAdjustment;
	Stats.Health = HealthAdjustment;
	Stats.SpeedMultiplier = SpeedAdjustment;

	PlayerModifier.StatModifications = Stats;
	PlayerModifier.IconID = CardID;
	PlayerModifier.MaxLifespan = Duration;
	PlayerModifier.ExpiredLifespan = 0;
	PlayerModifier.ResetOnEnd = ResetOnExpire;

	if (!IsValid(TargetCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find player characters"))
	}

	UPlayerModifierComponent* PlayerStats = TargetCharacter->GetPlayerModifierComponent();

	if (!IsValid(PlayerStats))
	{
		UE_LOG(LogTemp,Warning, TEXT("Could not find player stats"))
		return;
	}

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


FPlayerModifier UCardBase::GetPlayerModifier()
{
	return PlayerModifier;
}
