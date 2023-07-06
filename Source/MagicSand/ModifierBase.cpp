// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifierBase.h"

bool UModifierBase::CheckConstaints()
{
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (!IsValid(Constraint)) continue;

		if (!Constraint->Evaluate())
		{
			UE_LOG(LogTemp, Warning, TEXT("This constraint failed: %s in modifier %s"), *Constraint->GetName(), *GetName())
			return false;
		}
	}

	return true;
}

void UModifierBase::UpdateConstraints()
{
	for (UConstraintBase* Constraint : ConstraintArray)
	{
		if (!IsValid(Constraint)) continue;

		UE_LOG(LogTemp, Warning, TEXT("This constraint is processing: %s in modifier %s"), *Constraint->GetName(), *GetName())
		Constraint->ProcessFire();
	}
}

TArray< AActor*> UModifierBase::ProcessSingle_Implementation(AActor* Projectile)
{
	if (!IsValid(Projectile)) return TArray<AActor*>();
	
	// Do logic on projectile here

	return TArray<AActor*>({ Projectile });
}

TArray<AActor*> UModifierBase::ProcessProjectiles(TArray<AActor*> ProjectileArray)
{
	TArray<AActor*> Intermediary = TArray<AActor*>();

	for (AActor* Projectile: ProjectileArray)
	{
		
		if (!CheckConstaints()) continue;
		UpdateConstraints();

		TArray<AActor*> ResultProjectiles = ProcessSingle(Projectile);
		//Sanity checks go here (null safety, what are we modifying?)
		Intermediary.Append(ResultProjectiles);
	}

	return Intermediary;
}

void UModifierBase::Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	for (auto Constraint : ConstraintArray)
	{
		Constraint->Tick(DeltaTime, TickType, ThisTickFunction);
	}
}

void UModifierBase::AddConstraint(TSubclassOf<UConstraintBase> ConstraintClass)
{
	UConstraintBase* ConstraintObject = NewObject<UConstraintBase>(this, ConstraintClass);
	ConstraintArray.Add(ConstraintObject);
}

void UModifierBase::ClearConstraints()
{
	ConstraintArray.Empty();
}