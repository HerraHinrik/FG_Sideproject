// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerModifier.h"
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
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 CardID;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UModifierBase> WeaponModifier;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerModifier> PlayerModifier;

protected:
	void ApplyWeaponModifier(AFirstPersonViewCharacter* TargetCharacter);
	void ApplyPlayerModifier(AFirstPersonViewCharacter* TargetCharacter);

public:
	UFUNCTION(BlueprintCallable)
	void PlayCard(AFirstPersonViewCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable)
	int32 GetID();
};
