// By Amos Johan Persson


#include "ModifierBase.h"

UModifierBase::UModifierBase()
{
	ConstraintArray = TArray<UConstraintBase*>({ });

	bUsesTick = GIsRunning;
}



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

void UModifierBase::ModifierTick(float DeltaTime)
{
	LastTickFrame = GFrameCounter;
}

bool UModifierBase::CanTick() const
{
	return bUsesTick && (LastTickFrame != GFrameCounter);
}

TArray<AProjectileBase*> UModifierBase::ProcessSingle_Implementation(AProjectileBase* Projectile)
{
	if (!IsValid(Projectile)) return TArray<AProjectileBase*>();
	
	// Do logic on projectile here

	return TArray<AProjectileBase*>({ Projectile });
}

TArray<AProjectileBase*> UModifierBase::ProcessProjectiles(TArray<AProjectileBase*> ProjectileArray)
{
	TArray<AProjectileBase*> Intermediary = TArray<AProjectileBase*>();

	for (AProjectileBase* Projectile : ProjectileArray)
	{

		if (!CheckConstaints()) continue;
		UpdateConstraints();

		TArray<AProjectileBase*> ResultProjectiles = ProcessSingle(Projectile);
		//Sanity checks go here (null safety, what are we modifying?)
		Intermediary.Append(ResultProjectiles);
	}

	return Intermediary;
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