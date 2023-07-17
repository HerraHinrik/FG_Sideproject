// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstraintBase.h"
#include "SpawnerBase.h"
#include "ModifierBase.h"
#include "Loadout.generated.h"


/**
 * Responsible for all per-weapon logic. Only exists on the server. 
 */

UCLASS(Blueprintable)
class MAGICSAND_API ULoadout : public UObject
{
	GENERATED_BODY()

public:
	ULoadout();

private:
	UPROPERTY()
	TArray<UModifierBase*> ExpiredModifiers;

private:

	UFUNCTION()
	void RemoveSpawner(USpawnerBase* SpawnerObject);

	UFUNCTION()
	void RemoveConstraint(UConstraintBase* ConstraintObject);

	UFUNCTION()
	void RemoveModifier(UModifierBase* ModifierObject);

	UFUNCTION()
	void SlateModifierForRemoval(UModifierBase* ModifierObject);

protected:

	// Arrays of constructed gun part objects
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		TArray<USpawnerBase*> SpawnerArray;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		TArray<UConstraintBase*> ConstraintArray;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
		TArray<UModifierBase*> ModifierArray;


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

	void Fire(FVector Location, FRotator Rotation);

	//virtual void Tick(float DeltaTime);
};
