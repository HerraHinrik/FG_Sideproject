// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGunComponent::ClearLoadout(FWeaponLoadout Loadout)
{
	Loadout.ConstraintArray.Empty();
	Loadout.ModifierArray.Empty();
	Loadout.SpawnerArray.Empty();
}


void UGunComponent::RemoveSpawnerFromLoadout(USpawnerBase* SpawnerObject, FWeaponLoadout Loadout)
{
	Loadout.SpawnerArray.Remove(SpawnerObject);
}

void UGunComponent::RemoveConstraintFromLoadout(UConstraintBase* ConstraintObject, FWeaponLoadout Loadout)
{
	Loadout.ConstraintArray.Remove(ConstraintObject);
}

void UGunComponent::RemoveModifierFromLoadout(UModifierBase* ModifierObject, FWeaponLoadout Loadout)
{
	Loadout.ModifierArray.Remove(ModifierObject);
}

void UGunComponent::InitializeGunComponent()
{
	FWeaponLoadout Shotgun = BuildShotgunLoadout();
	FWeaponLoadout BoltAction = BuildBoltLoadout();

	AddLoadout(BoltAction);
	AddLoadout(Shotgun);
}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FWeaponLoadout Loadout : LoadoutArray)
	{ 

		for (UConstraintBase* Constraint : Loadout.ConstraintArray)
		{
			if (!IsValid(Constraint)) continue;


			Constraint->ConstraintTick(DeltaTime);
		}

		for (UModifierBase* Modifier : Loadout.ModifierArray)
		{
			if (!IsValid(Modifier)) continue;

			Modifier->ModifierTick(DeltaTime);
		}
	}
}

void UGunComponent::RegisterReloadSubscribers(FWeaponLoadout Loadout)
{
	auto Constraints = Loadout.ConstraintArray;
	for (auto Constraint : Constraints)
	{
		if (!IsValid(Constraint))
		{
			UE_LOG(LogTemp,Warning, TEXT("Found invalid constraint in: %d"), CurrentIndex)
			continue;
		}

		// Empty response function on ConstraintBase allows children to add custom reload response
		OnReload.AddDynamic(Constraint, &UConstraintBase::OnReload);
	}
}

void UGunComponent::ClearReloadSubscribers()
{
	OnReload.Clear();
}


void UGunComponent::ToggleLoadout()
{
	if (LoadoutArray.IsEmpty())
	{
		return;
	}

	ClearReloadSubscribers();

	int32 Index = CurrentIndex;
	Index ++;
	Index %= LoadoutArray.Num();
	CurrentIndex = Index;

	RegisterReloadSubscribers(LoadoutArray[CurrentIndex]);
}


void UGunComponent::AddLoadout(FWeaponLoadout Loadout)
{
	ClearReloadSubscribers();
	RegisterReloadSubscribers(Loadout);
	Reload();

	int32 Index = LoadoutArray.Add(Loadout);
	CurrentIndex = Index;

}

void UGunComponent::ApplyModifier(TSubclassOf<UModifierBase> ModifierClass)
{
	UModifierBase* NewModifier = CreateModifier(ModifierClass);
	LoadoutArray[CurrentIndex].ModifierArray.Add(NewModifier);
}

void UGunComponent::Fire()
{
	UCameraComponent* origin = GetOwner()->FindComponentByClass<UCameraComponent>();

	if (!IsValid(origin)) return;

	FRotator Rotation = origin->GetComponentRotation();
	FVector Location = origin->GetComponentLocation();

	auto Constraints = LoadoutArray[CurrentIndex].ConstraintArray;
	auto Modifiers = LoadoutArray[CurrentIndex].ModifierArray;
	auto Spawners = LoadoutArray[CurrentIndex].SpawnerArray;

	// Constraints
	for (UConstraintBase* Constraint : Constraints)
	{
		if (!IsValid(Constraint)) continue;

		UE_LOG(LogTemp, Warning, TEXT("Constraint is evaluating: %s"), *Constraint->GetName())

		if (!Constraint->Evaluate())
		{
			UE_LOG(LogTemp, Warning, TEXT("This constraint failed: %s"), *Constraint->GetName())
				return;
		}
	}

	// Update constraints internal logic
	for (UConstraintBase* Constraint : Constraints)
	{
		if (!IsValid(Constraint)) continue;

		UE_LOG(LogTemp, Warning, TEXT("This constraint is processing fire: %s"), *Constraint->GetName())
			Constraint->ProcessFire();

	}

	// Spawning
	TArray<AProjectileBase*> NewProjectiles = TArray<AProjectileBase*>();

	for (USpawnerBase* Spawner : Spawners)
	{
		if (!IsValid(Spawner)) continue;

		NewProjectiles.Append(Spawner->SpawnProjectiles(Location, Rotation));
	}

	// Modifiers
	for (UModifierBase* Modifier : Modifiers)
	{
		if (!IsValid(Modifier)) continue;

		TArray<AProjectileBase*> OutProjectiles = Modifier->ProcessProjectiles(NewProjectiles);
		NewProjectiles = OutProjectiles;
	}
}

void UGunComponent::Reload()
{
	OnReload.Broadcast();
}