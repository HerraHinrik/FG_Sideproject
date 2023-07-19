// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModifierStructs.h"
#include "PlayerModifier.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerModifierDelegate, UPlayerModifier*, ModifierObject);

UCLASS()
class MAGICSAND_API UPlayerModifier : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnExpire;
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock StatModifications;

	UPROPERTY(BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float ExpiredDuration = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float LifetimeDuration;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 IconID;

public:
	void TickModifier(float DeltaTime);

	UFUNCTION()
	FPlayerStatBlock GetStatModifications();


	UFUNCTION(BlueprintCallable)
		int32 GetIconID();

	UFUNCTION(BlueprintCallable)
		float GetDurationLeft();

	UFUNCTION(BlueprintCallable)
		float GetDurationMax();
};
