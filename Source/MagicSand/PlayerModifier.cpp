// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerModifier.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerModifier::TickModifier(float DeltaTime)
{
	// For child classes to override
}

void UPlayerModifier::InitializeStatValues()
{
	StatModifications = FPlayerStatBlock();

	StatModifications.Armor = Armor;
	StatModifications.DamageFlat = DamageFlat;
	StatModifications.DamageMultiplier = DamageMultiplier;
	StatModifications.Health = Health;
	StatModifications.SpeedMultiplier = SpeedMultiplier;
}

FPlayerStatBlock UPlayerModifier::GetStatModifications()
{
	return StatModifications;
}


int32 UPlayerModifier::GetIconID()
{
	return IconID;
}

float UPlayerModifier::GetDurationLeft()
{
	float TimeLeft = LifetimeDuration - ExpiredDuration;
	return UKismetMathLibrary::Max(0, TimeLeft);
}

float UPlayerModifier::GetDurationMax()
{
	return LifetimeDuration;
}

bool UPlayerModifier::ResetsOnExpire()
{
	return ResetOnExpire;
}