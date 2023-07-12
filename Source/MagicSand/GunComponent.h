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

public: // Properties

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<ULoadout*> LoadoutArray;

public: // Functions

	UFUNCTION(BlueprintCallable)
	void RegisterReloadSubscribers(ULoadout* Loadout);

	void SetCurrentLoadoutByIndex(int Index);

	UFUNCTION(BlueprintCallable)
	void ToggleLoadout();

	UFUNCTION(BlueprintCallable)
	void AddLoadout(ULoadout* Loadout);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void BuildShotgunLoadoutIn(ULoadout* TargetLoadout);
	//virtual ULoadout* BuildShotgunLoadout_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BuildDiscLoadoutIn(ULoadout* TargetLoadout);
	//virtual ULoadout* BuildDiscLoadout_Implementation();

	UFUNCTION(BlueprintImplementableEvent)
		void BuildBoltLoadoutIn(ULoadout* TargetLoadout);
	//virtual ULoadout* BuildBoltLoadout_Implementation();

};
