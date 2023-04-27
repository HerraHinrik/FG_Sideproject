// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifierBase.h"


void UModifierBase::ProcessSingle_Implementation(AActor* Projectile)
{
	if (IsValid(Projectile))
	{
		for (auto Constraint : ConstraintArray)
		{
			Constraint->ProcessFire();
		}

		Projectile->AddComponentByClass(TargetComponent, false, FTransform(), false);
		//if (auto Component = Projectile->GetComponentByClass(TargetComponent))
		// // Do logic on component here
	}

	
}

void UModifierBase::ProcessProjectiles(TArray<AActor*> ProjectileArray)
{
	for (AActor* Projectile: ProjectileArray)
	{
		ProcessSingle(Projectile);
	}
}

void UModifierBase::Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (auto Constraint : ConstraintArray)
	{
		Constraint->Tick(DeltaTime, TickType, ThisTickFunction);
	}
}