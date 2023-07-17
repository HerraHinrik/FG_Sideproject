// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerBase.h"
#include "ConstraintBase.h"
#include "ModifierBase.h"
#include "Loadout.h"
#include "GunComponent.generated.h"

/**
 * Handles communication between player input and loadouts on the server. Also responsible for building loadouts.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGunDelegate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGICSAND_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()


public:	// Constructors and Delegates only
	UGunComponent();

	UPROPERTY(BlueprintAssignable);
	FGunDelegate OnReload;

protected: // Properties
	
	ULoadout* CurrentLoadout;

	int CurrentIndex = 0;

protected: // Functions
	virtual void BeginPlay() override;

public: // Properties
	TArray<ULoadout*> LoadoutArray;

public: // Functions

	void RegisterReloadSubscribers(ULoadout* Loadout);

	void ClearReloadSubscribers();

	void SetCurrentLoadoutByIndex(int Index);

	UFUNCTION(BlueprintCallable)
	void ToggleLoadout();

	UFUNCTION(BlueprintCallable)
	void AddLoadout(ULoadout* Loadout);

	UFUNCTION(BlueprintCallable)
	void ApplyModifier(TSubclassOf<UModifierBase> Modifier);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintImplementableEvent)
		void BuildShotgunLoadout(ULoadout* Target);
	//virtual ULoadout* BuildShotgunLoadout_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BuildDiscLoadout(ULoadout* Target);
	//virtual ULoadout* BuildDiscLoadout_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BuildBoltLoadout(ULoadout* Target);
	//virtual ULoadout* BuildBoltLoadout_Implementation();
};
