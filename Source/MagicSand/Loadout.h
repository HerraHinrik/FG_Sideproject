// By Amos Johan Persson

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
	UFUNCTION(BlueprintCallable)
		void AddSpawner(TSubclassOf<USpawnerBase> SpawnerClass);

	UFUNCTION(BlueprintCallable)
		void AddConstraint(TSubclassOf<UConstraintBase> ConstraintClass);

	UFUNCTION(BlueprintCallable)
		void AddModifier(TSubclassOf<UModifierBase> ModifierClass);

	UFUNCTION(BlueprintCallable)
		void Clear();

	TArray<USpawnerBase*> GetSpawners();
	TArray<UConstraintBase*> GetConstraints();
	TArray<UModifierBase*> GetModifiers();
};
