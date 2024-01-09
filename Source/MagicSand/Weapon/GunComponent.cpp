// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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



void UGunComponent::ClearReloadSubscribers()
{
	OnReload.Clear();
}


void UGunComponent::RemoveSpawnerFromLoadout(USpawnerBase* SpawnerObject, int32 Index)
{
	auto Loadout = LoadoutArray[Index];
	Loadout.SpawnerArray.Remove(SpawnerObject);
}

void UGunComponent::RemoveConstraintFromLoadout(UConstraintBase* ConstraintObject, int32 Index)
{
	auto Loadout = LoadoutArray[Index];
	Loadout.ConstraintArray.Remove(ConstraintObject);
}

void UGunComponent::RemoveModifierFromLoadout(UModifierBase* ModifierObject, int32 Index)
{
	auto Loadout = LoadoutArray[Index];
	int32 Amount = Loadout.ModifierArray.Remove(ModifierObject);

	UE_LOG(LogTemp, Warning, TEXT("Destroyed %d modifiers."), Amount)
}

void UGunComponent::CreateWeaponLoadouts_Implementation()
{
	FWeaponLoadout Shotgun = BuildShotgunLoadout();
	FWeaponLoadout BoltAction = BuildBoltLoadout();

	UE_LOG(LogTemp, Warning, TEXT("Creating weapons"))
	AddLoadout(BoltAction);
	AddLoadout(Shotgun);
}

bool UGunComponent::CreateWeaponLoadouts_Validate()
{
	return true;
}

void UGunComponent::InitializeGunComponent(UPlayerModifierComponent* PlayerStatsComponent)
{
	PlayerStats = PlayerStatsComponent;

	CreateWeaponLoadouts();

	SetNetAddressable();
	SetIsReplicated(true);
}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FWeaponLoadout& Loadout : LoadoutArray)
	{ 
		TArray<UConstraintBase*> &Constraints = Loadout.ConstraintArray;
		TArray<UModifierBase*> &Modifiers = Loadout.ModifierArray;
		TArray<float> &Durations = Loadout.ModifierDurations;

		for (UConstraintBase* Constraint : Constraints)
		{
			if (!IsValid(Constraint))
			{

				continue;
			}

			Constraint->ConstraintTick(DeltaTime);
		}

		for (int i = 0; i < Modifiers.Num(); i++)
		{
			if (!IsValid(Modifiers[i]))
			{

				continue;
			}

			if (!Durations.IsValidIndex(i))
			{

					continue;
			}

			Durations[i] += DeltaTime;

			if (Durations[i] >= Modifiers[i]->GetDurationMax())
			{
				Loadout.ExpiredModifiers.Add(Modifiers[i]);
			}
		}
		
		for (UModifierBase* Modifier : Loadout.ExpiredModifiers)
		{
			int32 Index = Modifiers.IndexOfByKey(Modifier);

			Loadout.ModifierArray.RemoveAt(Index);
			Loadout.ModifierDurations.RemoveAt(Index);
		}

		Loadout.ExpiredModifiers.Empty();
	}
}

void UGunComponent::ToggleLoadout_Implementation()
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

bool UGunComponent::ToggleLoadout_Validate()
{
	return true;
}

void UGunComponent::ApplyModifier_Implementation(TSubclassOf<UModifierBase> ModifierClass)
{
	UModifierBase* NewModifier = CreateModifier(ModifierClass);
	FWeaponLoadout& Loadout = LoadoutArray[CurrentIndex];
	Loadout.ModifierArray.Add(NewModifier);
	Loadout.ModifierDurations.Add(0);
}

bool UGunComponent::ApplyModifier_Validate(TSubclassOf<UModifierBase> ModifierClass)
{
	return true;
}

void UGunComponent::Fire_Implementation()
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

		//UE_LOG(LogTemp, Warning, TEXT("Constraint is evaluating: %s"), *Constraint->GetName())

		if (!Constraint->Evaluate())
		{
			//UE_LOG(LogTemp, Warning, TEXT("This constraint failed: %s"), *Constraint->GetName())
				return;
		}
	}

	// Update constraints internal logic
	for (UConstraintBase* Constraint : Constraints)
	{
		if (!IsValid(Constraint)) continue;

		//UE_LOG(LogTemp, Warning, TEXT("This constraint is processing fire: %s"), *Constraint->GetName())
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

bool UGunComponent::Fire_Validate()
{
	return true;
}

void UGunComponent::Reload_Implementation()
{
	OnReload.Broadcast();
}

bool UGunComponent::Reload_Validate()
{
	return true;
}

TArray<FModifierUIData> UGunComponent::GetActiveModifierData()
{
	TArray<FModifierUIData> Results;

	FWeaponLoadout Loadout = LoadoutArray[CurrentIndex];
	TArray<UModifierBase*> Modifiers = Loadout.ModifierArray;

	for (int32 i = 0; i < Modifiers.Num(); i++)
	{
		auto Modifier = Modifiers[i];

		if (!IsValid(Modifier))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Invalid modifier pointer while requesting UI data"))
			continue;
		}

		FModifierUIData Info = FModifierUIData();
		Info.IconID = Modifier->GetIconID();
		Info.DurationMax = Modifier->GetDurationMax();
		Info.DurationLeft = Info.DurationMax - Loadout.ModifierDurations[i];

		Results.Add(Info);
	}

	return Results;
}

void UGunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(UGunComponent, LoadoutArray);
}