// By Amos Johan Persson

#include "Loadout.h"

ULoadout::ULoadout()
{
	SpawnerArray = TArray<USpawnerBase*>();
	ConstraintArray = TArray<UConstraintBase*>();
	ModifierArray = TArray<UModifierBase*>();
}


void ULoadout::RemoveSpawner(USpawnerBase* SpawnerObject)
{
	//I was here
}

void ULoadout::RemoveConstraint(UConstraintBase* ConstraintObject)
{

}

void ULoadout::RemoveModifier(UModifierBase* ModifierObject)
{
	ModifierArray.Remove(ModifierObject);
}

void ULoadout::SlateModifierForRemoval(UModifierBase* ModifierObject)
{
	ExpiredModifiers.AddUnique(ModifierObject);
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

	UModifierBase* ModifierObject = NewObject<UModifierBase>(this, ModifierClass);
	ModifierArray.Add(ModifierObject);

	ModifierObject->OnHasExpired.AddDynamic(this, &ULoadout::SlateModifierForRemoval);
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
	//Clear expired modifiers before we process fire
	for (UModifierBase* Modifier : ExpiredModifiers)
	{
		RemoveModifier(Modifier);
		Modifier->Cleanup();
	}
	ExpiredModifiers.Empty();

	// Constraints
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (!IsValid(Constraint)) continue;

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

		UE_LOG(LogTemp, Warning, TEXT("This constraint is processing: %s"), *Constraint->GetName())
			Constraint->ProcessFire();

	}

	// Spawning
	TArray<AProjectileBase*> NewProjectiles = TArray<AProjectileBase*>();


	for (USpawnerBase* Spawner : SpawnerArray)
	{
		if (!IsValid(Spawner)) continue;

		NewProjectiles.Append(Spawner->SpawnProjectiles(Location, Rotation));
	}

	// Modifiers
	for (UModifierBase* Modifier : ModifierArray)
	{
		if (!IsValid(Modifier)) continue;

		TArray<AProjectileBase*> OutProjectiles = Modifier->ProcessProjectiles(NewProjectiles);
		NewProjectiles = OutProjectiles;
	}
}

//void  ULoadout::Tick(float DeltaTime)
//{
//	for (UConstraintBase* Constraint : ConstraintArray)
//	{
//		if (!IsValid(Constraint)) continue;
//
//		Constraint->Tick(DeltaTime);
//	}
//
//	for (UModifierBase* Modifier : ModifierArray)
//	{
//		if (!IsValid(Modifier)) continue;
//
//		Modifier->Tick(DeltaTime);
//	}
//}