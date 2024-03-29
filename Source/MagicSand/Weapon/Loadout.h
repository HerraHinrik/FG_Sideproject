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

private:

	UFUNCTION()
	void RemoveSpawner(USpawnerBase* SpawnerObject);

	UFUNCTION()
	void RemoveConstraint(UConstraintBase* ConstraintObject);

	UFUNCTION()
	void RemoveModifier(UModifierBase* ModifierObject);

protected:

	// Arrays of constructed gun part objects
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<USpawnerBase*> SpawnerArray;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UConstraintBase*> ConstraintArray;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TArray<UModifierBase*> ModifierArray;


public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void AddSpawner(TSubclassOf<USpawnerBase> SpawnerClass);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void AddConstraint(TSubclassOf<UConstraintBase> ConstraintClass);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void AddModifier(TSubclassOf<UModifierBase> ModifierClass);

	UFUNCTION(BlueprintCallable)
		void Clear();

	UFUNCTION(BlueprintCallable)
	TArray<USpawnerBase*> GetSpawners();

	UFUNCTION(BlueprintCallable)
	TArray<UConstraintBase*> GetConstraints();

	UFUNCTION(BlueprintCallable)
	TArray<UModifierBase*> GetModifiers();

	void Fire(FVector Location, FRotator Rotation);

	void TickLoadout(float DeltaTime);
};
