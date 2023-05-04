// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerBase.h"
#include "ConstraintBase.h"
#include "ModifierBase.h"
#include "Loadout.h"
#include "GunComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGunDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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

	void SetCurrentLoadout(int Index);

	UFUNCTION(BlueprintCallable)
	void ToggleLoadout();

	UFUNCTION(BlueprintCallable)
	void AddLoadout(ULoadout* Loadout);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
