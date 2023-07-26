// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModifierStructs.h"
#include "PlayerModifier.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerModifierDelegateDeprecated, UPlayerModifierOld*, ModifierObject);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class MAGICSAND_API UPlayerModifierOld : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegateDeprecated OnExpire;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock StatModifications;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ExpiredDuration = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float LifetimeDuration;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int32 IconID;

public:
	void TickModifier(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	FPlayerStatBlock GetStatModifications();


	UFUNCTION(BlueprintCallable)
	int32 GetIconID();

	UFUNCTION(BlueprintCallable)
	float GetDurationLeft();

	UFUNCTION(BlueprintCallable)
	float GetDurationMax();
};
