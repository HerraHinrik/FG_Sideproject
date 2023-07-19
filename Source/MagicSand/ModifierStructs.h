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

	UPROPERTY()
	float SpeedMultiplier;

	UPROPERTY()
	float DamageMultiplier;

	UPROPERTY()
	int32 DamageFlat;

	UPROPERTY()
	int32 Armor;

	UPROPERTY()
	float Health;
};
