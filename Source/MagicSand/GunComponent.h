// By Amos Johan Persson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnerBase.h"
#include "ConstraintBase.h"
#include "ModifierBase.h"
#include "Loadout.h"
#include "ModifierStructs.h"
#include "PlayerModifierComponent.h"
#include "GunComponent.generated.h"

USTRUCT(BlueprintType)
struct FWeaponLoadout
{
	GENERATED_BODY()

	// Arrays of constructed gun part objects
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<USpawnerBase*> SpawnerArray;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UConstraintBase*> ConstraintArray;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UModifierBase*> ModifierArray;
};

/**
 * Handles communication between player input and loadouts on the server. Also responsible for building loadouts.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGunDelegate);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGICSAND_API UGunComponent : public UActorComponent
{
	GENERATED_BODY()


public:	// Constructors and Delegates only
	UGunComponent();

	UPROPERTY(BlueprintAssignable);
	FGunDelegate OnReload;

protected: // Properties
	UPROPERTY()
	int32 CurrentIndex = 0;

	UPROPERTY()
	TArray<FWeaponLoadout> LoadoutArray;


	UPROPERTY()
	UPlayerModifierComponent* PlayerStats;

protected: // Functions
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	USpawnerBase* CreateSpawner(TSubclassOf<USpawnerBase> SpawnerClass);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UConstraintBase* CreateConstraint(TSubclassOf<UConstraintBase> ConstraintClass);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UModifierBase* CreateModifier(TSubclassOf<UModifierBase> ModifierClass);

	UFUNCTION(BlueprintCallable)
	void ClearLoadout(FWeaponLoadout Loadout);


	UFUNCTION()
	void RemoveSpawnerFromLoadout(USpawnerBase* SpawnerObject, FWeaponLoadout Loadout);

	UFUNCTION()
	void RemoveConstraintFromLoadout(UConstraintBase* ConstraintObject, FWeaponLoadout Loadout);

	UFUNCTION()
	void RemoveModifierFromLoadout(UModifierBase* ModifierObject, FWeaponLoadout Loadout);

public: // Properties

public: // Functions

	void InitializeGunComponent(UPlayerModifierComponent* PlayerStatsComponent);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RegisterReloadSubscribers(FWeaponLoadout Loadout);

	void ClearReloadSubscribers();

	UFUNCTION(BlueprintCallable)
	void ToggleLoadout();

	UFUNCTION(BlueprintCallable)
	void AddLoadout(FWeaponLoadout Loadout);

	UFUNCTION(BlueprintCallable)
	void ApplyModifier(TSubclassOf<UModifierBase> Modifier);

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintImplementableEvent)
	FWeaponLoadout BuildShotgunLoadout();

	UFUNCTION(BlueprintImplementableEvent)
	FWeaponLoadout BuildDiscLoadout();

	UFUNCTION(BlueprintImplementableEvent)
	FWeaponLoadout BuildBoltLoadout();

	// UI facing interface
	UFUNCTION(BlueprintCallable)
	TArray<FModifierUIData> GetActiveModifierData();
};