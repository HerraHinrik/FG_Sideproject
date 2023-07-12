// By Amos Johan Persson

#include "Loadout.h"

ULoadout::ULoadout()
{
	SpawnerArray = TArray<USpawnerBase*>();
	ConstraintArray = TArray<UConstraintBase*>();
	ModifierArray = TArray<UModifierBase*>();
}

void ULoadout::AddSpawner(TSubclassOf<USpawnerBase> SpawnerClass)
{

	USpawnerBase* SpawnerObject = NewObject<USpawnerBase>(this, SpawnerClass);
	SpawnerArray.Add(SpawnerObject);
}

void ULoadout::AddConstraint(TSubclassOf<UConstraintBase> ConstraintClass)
{

	UConstraintBase* ConstraintObject = NewObject<UConstraintBase>(this, ConstraintClass);
	ConstraintArray.Add(ConstraintObject);
}

void ULoadout::AddModifier(TSubclassOf<UModifierBase> ModifierClass)
{

	UModifierBase* SpawnerObject = NewObject<UModifierBase>(this, ModifierClass);
	ModifierArray.Add(SpawnerObject);
}


void ULoadout::Clear()
{
	SpawnerArray.Empty();
	ConstraintArray.Empty();
	ModifierArray.Empty();
}

TArray<USpawnerBase*> ULoadout::GetSpawners()
{
	return SpawnerArray;
}

TArray<UConstraintBase*> ULoadout::GetConstraints()
{
	return ConstraintArray;
}

TArray<UModifierBase*> ULoadout::GetModifiers()
{
	return ModifierArray;
}

void ULoadout::Fire(FVector Location, FRotator Rotation)
{
	// Constraints
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (Constraint == nullptr) continue;

		if (!Constraint->Evaluate())
		{
			UE_LOG(LogTemp, Warning, TEXT("This constraint failed: %s"), *Constraint->GetName())
				return;
		}
	}

	// Update constraints internal logic
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (Constraint == nullptr) continue;

		UE_LOG(LogTemp, Warning, TEXT("This constraint is processing: %s"), *Constraint->GetName())
			Constraint->ProcessFire();

	}

	// Spawning
	TArray<AProjectileBase*> NewProjectiles = TArray<AProjectileBase*>();


	for (USpawnerBase* Spawner : SpawnerArray)
	{
		NewProjectiles.Append(Spawner->SpawnProjectiles(Location, Rotation));
	}

	// Modifiers
	for (UModifierBase* Modifier : ModifierArray)
	{
		TArray<AProjectileBase*> OutProjectiles = Modifier->ProcessProjectiles(NewProjectiles);
		NewProjectiles = OutProjectiles;
	}
}

void  ULoadout::Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		Constraint->Tick(DeltaTime, TickType, ThisTickFunction);
	}
}