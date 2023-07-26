// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModifierStructs.h"
#include "CardBase.generated.h"

class AFirstPersonViewCharacter;
class UModifierBase;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MAGICSAND_API UCardBase : public UObject
{
	GENERATED_BODY()

public:
	//UCardBase();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 CardID;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Duration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool ResetOnExpire;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UModifierBase> WeaponModifier;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float HealthAdjustment;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float SpeedAdjustment;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 RawDamageAdjustment;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float DamageMultiplierAdjustment;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ArmorAdjustment;

	UPROPERTY(BlueprintReadWrite)
	FPlayerModifier PlayerModifier;

protected:
	void ApplyWeaponModifier(AFirstPersonViewCharacter* TargetCharacter);
	void ApplyPlayerModifier(AFirstPersonViewCharacter* TargetCharacter);

public:
	UFUNCTION(BlueprintCallable)
	void PlayCard(AFirstPersonViewCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	int32 GetID();

	UFUNCTION()
	FPlayerModifier GetPlayerModifier();
};
