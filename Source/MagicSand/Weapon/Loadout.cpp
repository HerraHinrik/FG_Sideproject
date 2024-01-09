// By Amos Johan Persson

#include "Loadout.h"

ULoadout::ULoadout()
{
	ConstraintArray.Init(nullptr, 1);
	ModifierArray.Init(nullptr, 1);
	SpawnerArray.Init(nullptr, 1);
}


void ULoadout::RemoveSpawner(USpawnerBase* SpawnerObject)
{

}

void ULoadout::RemoveConstraint(UConstraintBase* ConstraintObject)
{

}

void ULoadout::RemoveModifier(UModifierBase* ModifierObject)
{
	ModifierArray.RemoveSingle(ModifierObject);
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
		if (!IsValid(Constraint)) continue;

		UE_LOG(LogTemp, Warning, TEXT("Constraint is evaluating: %s"), *Constraint->GetName())

		if (!Constraint->Evaluate())
		{
			UE_LOG(LogTemp, Warning, TEXT("This constraint failed: %s"), *Constraint->GetName())
			return;
		}
	}

	// Update constraints internal logic
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (!IsValid(Constraint)) continue;

		UE_LOG(LogTemp, Warning, TEXT("This constraint is processing fire: %s"), *Constraint->GetName())
		Constraint->ProcessFire();

	}

	// Spawning
	TArray<AProjectileBase*> NewProjectiles = TArray<AProjectileBase*>();


	for (USpawnerBase* Spawner : SpawnerArray)
	{
		if (!IsValid(Spawner)) continue;

		//NewProjectiles.Append(Spawner->SpawnProjectiles(Location, Rotation));
	}

	// Modifiers
	for (UModifierBase* Modifier : ModifierArray)
	{
		if (!IsValid(Modifier)) continue;

		TArray<AProjectileBase*> OutProjectiles = Modifier->ProcessProjectiles(NewProjectiles);
		NewProjectiles = OutProjectiles;
	}
}

void ULoadout::TickLoadout(float DeltaTime)
{

	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (!IsValid(Constraint)) continue;
		
		
		Constraint->ConstraintTick(DeltaTime);
	}
		
	for (UModifierBase* Modifier : ModifierArray)
	{
		if (!IsValid(Modifier)) continue;
		
		//Modifier->ModifierTick(DeltaTime);
	}
}
