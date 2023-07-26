// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModifierStructs.generated.h"

USTRUCT(BlueprintType)
struct FModifierUIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 IconID = 0;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DurationLeft = 0;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DurationMax = 0;
};

USTRUCT(BlueprintType)
struct FPlayerStatBlock
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SpeedMultiplier = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DamageMultiplier = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 DamageFlat = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Armor = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Health = 0;

	inline bool operator==(const FPlayerStatBlock& other) const
	{
		if (SpeedMultiplier != other.SpeedMultiplier) return false;
		if (DamageMultiplier != other.DamageMultiplier) return false;
		if (DamageFlat != other.DamageFlat) return false;
		if (Armor != other.Armor) return false;
		if (Health != other.Health) return false;

		return true;
	}
};

USTRUCT(BlueprintType)
struct FPlayerModifier
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock StatModifications;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float MaxLifespan = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ExpiredLifespan = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool ResetOnEnd = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 IconID = 0;

	inline bool operator==(const FPlayerModifier& other) const
	{
		return StatModifications == other.StatModifications && ExpiredLifespan == other.ExpiredLifespan;
	}
};