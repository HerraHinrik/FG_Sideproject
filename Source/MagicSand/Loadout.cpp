// Fill out your copyright notice in the Description page of Project Settings.

#include "Loadout.h"

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