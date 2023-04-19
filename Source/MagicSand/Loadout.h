// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstraintBase.h"
#include "SpawnerBase.h"
#include "ModifierBase.h"
#include "Loadout.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class MAGICSAND_API ULoadout : public UObject
{
	GENERATED_BODY()

protected:

	// Arrays of constructed gun part objects
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		TArray<USpawnerBase*> SpawnerArray;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		TArray<UConstraintBase*> ConstraintArray;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		TArray<UModifierBase*> ModifierArray;

public:


public:

	TArray<USpawnerBase*>* GetSpawners();
	TArray<UConstraintBase*>* GetConstraints();
	TArray<UModifierBase*>* GetModifiers();
};
