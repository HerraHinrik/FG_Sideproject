// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModifierStructs.generated.h"

USTRUCT(BlueprintType)
struct FModifierUIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 IconID;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DurationLeft;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DurationMax;
};

USTRUCT(BlueprintType)
struct FPlayerStatBlock
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float SpeedMultiplier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float DamageMultiplier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 DamageFlat;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 Armor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Health;

	/*FPlayerStatBlock()
	{
		SpeedMultiplier = 0;
		DamageMultiplier = 0;
		DamageFlat = 0;
		Armor = 0;
		Health = 0;
	}*/
};
