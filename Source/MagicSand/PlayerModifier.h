// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapon/ModifierStructs.h"
#include "PlayerModifier.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerModifierDelegate, UPlayerModifier*, ModifierObject);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class MAGICSAND_API UPlayerModifier : public UObject
{
	GENERATED_BODY()
public:

	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock StatModifications;

	// Initial stat values
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
	//

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool ResetOnExpire;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ExpiredDuration = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float LifetimeDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 IconID;

public:
	virtual void TickModifier(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void InitializeStatValues();

	UFUNCTION(BlueprintCallable)
	FPlayerStatBlock GetStatModifications();

	UFUNCTION(BlueprintCallable)
	int32 GetIconID();

	UFUNCTION(BlueprintCallable)
	float GetDurationLeft();

	UFUNCTION(BlueprintCallable)
	float GetDurationMax();

	UFUNCTION(BlueprintCallable)
		bool ResetsOnExpire();
};
