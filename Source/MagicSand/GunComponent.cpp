// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponent();
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UGunComponent::AddLoadout(FWeaponLoadout Loadout)
{
	ClearReloadSubscribers();
	RegisterReloadSubscribers(Loadout);
	Reload();

	int32 Index = LoadoutArray.Add(Loadout);
	CurrentIndex = Index;
}

//bool UGunComponent::AddLoadout_Validate(FWeaponLoadout Loadout)
//{
//	return true;
//}

void UGunComponent::EmptyLoadout(FWeaponLoadout Loadout)
{
	Loadout.ConstraintArray.Empty();
	Loadout.ModifierArray.Empty();
	Loadout.SpawnerArray.Empty();
}

void UGunComponent::RegisterReloadSubscribers(FWeaponLoadout Loadout)
{
	auto Constraints = Loadout.ConstraintArray;
	for (auto Constraint : Constraints)
	{
		if (!IsValid(Constraint))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found invalid constraint in: %d"), CurrentIndex)
				continue;
		}

		// Empty response function on ConstraintBase allows children to add custom reload response
		OnReload.AddDynamic(Constraint, &UConstraintBase::OnReload);
	}
}

//bool UGunComponent::RegisterReloadSubscribers_Validate(FWeaponLoadout Loadout)
//{
//	return true;
//}

void UGunComponent::ClearReloadSubscribers()
{
	OnReload.Clear();
}

//bool UGunComponent::ClearReloadSubscribers_Validate()
//{
//	return true;
//}

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


void UGunComponent::CreateWeaponLoadouts()
{
	FWeaponLoadout Shotgun = BuildShotgunLoadout();
	FWeaponLoadout BoltAction = BuildBoltLoadout();

	AddLoadout(BoltAction);
	AddLoadout(Shotgun);
}

//bool UGunComponent::CreateWeaponLoadouts_Validate()
//{
//	return true;
//}

void UGunComponent::InitializeGunComponent(UPlayerModifierComponent* PlayerStatsComponent)
{
	SetNetAddressable();
	SetIsReplicated(true);

	PlayerStats = PlayerStatsComponent;

	CreateWeaponLoadouts();
}

//bool UGunComponent::InitializeGunComponent_Validate(UPlayerModifierComponent* PlayerStatsComponent)
//{
//	return true;
//}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FWeaponLoadout Loadout : LoadoutArray)
	{ 

		for (UConstraintBase* Constraint : Loadout.ConstraintArray)
		{
			if (!IsValid(Constraint))
			{
				UE_LOG(LogTemp, Warning, TEXT("Found invalid constraint in: %d"), CurrentIndex)

				continue;
			}

			Constraint->ConstraintTick(DeltaTime);
		}

		for (UModifierBase* Modifier : Loadout.ModifierArray)
		{
			if (!IsValid(Modifier))
			{
				UE_LOG(LogTemp, Warning, TEXT("Found invalid modifier in: %d"), CurrentIndex)

				continue;
			}

			Modifier->ModifierTick(DeltaTime);
		}
	}
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

//bool UGunComponent::ToggleLoadout_Validate()
//{
//	return true;
//}

void UGunComponent::ApplyModifier(TSubclassOf<UModifierBase> ModifierClass)
{
	UModifierBase* NewModifier = CreateModifier(ModifierClass);
	LoadoutArray[CurrentIndex].ModifierArray.Add(NewModifier);
}

//bool UGunComponent::ApplyModifier_Validate(TSubclassOf<UModifierBase> ModifierClass)
//{
//	return true;
//}

void UGunComponent::Fire()
{
	UCameraComponent* origin = GetOwner()->FindComponentByClass<UCameraComponent>();
	FPlayerStatBlock CurrentStats = PlayerStats->GetCurrentModifications();

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

		NewProjectiles.Append(Spawner->SpawnProjectiles(Location, Rotation, CurrentStats.DamageFlat, CurrentStats.DamageMultiplier));
	}

	// Modifiers
	for (UModifierBase* Modifier : Modifiers)
	{
		if (!IsValid(Modifier)) continue;

		TArray<AProjectileBase*> OutProjectiles = Modifier->ProcessProjectiles(NewProjectiles);
		NewProjectiles = OutProjectiles;
	}
}

//bool UGunComponent::Fire_Validate()
//{
//	return true;
//}

void UGunComponent::Reload()
{
	OnReload.Broadcast();
}

//bool UGunComponent::Reload_Validate()
//{
//	return true;
//}

TArray<FModifierUIData> UGunComponent::GetActiveModifierData()
{
	TArray<FModifierUIData> Results;

	TArray<UModifierBase*> Modifiers = LoadoutArray[CurrentIndex].ModifierArray;
	for (auto Modifier : Modifiers)
	{
		if (!IsValid(Modifier))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid modifier pointer while requesting UI data"))
			continue;
		}

		FModifierUIData Info = FModifierUIData();
		Info.IconID = Modifier->GetIconID();
		Info.DurationLeft = Modifier->GetDurationLeft();
		Info.DurationMax = Modifier->GetDurationMax();

		Results.Add(Info);
	}

	return Results;
}