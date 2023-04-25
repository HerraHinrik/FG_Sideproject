// Fill out your copyright notice in the Description page of Project Settings.

#include "Loadout.h"


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