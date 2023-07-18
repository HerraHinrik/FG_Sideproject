// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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

protected:
	UPROPERTY()
	uint32 CardID;

	TSubclassOf<UModifierBase> WeaponModifier;
	// Usomeclass playermodifier

protected:
	void ApplyWeaponModifier(AFirstPersonViewCharacter* TargetCharacter);
	//void ApplyPlayerModifier(AFirstPersonViewCharacter* TargetCharacter);

public:
	UFUNCTION(BlueprintCallable)
	void PlayCard(AFirstPersonViewCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	uint32 GetID();
};
