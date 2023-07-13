// By Amos Johan Persson


#include "ModifierBase.h"

UModifierBase::UModifierBase()
{
	Timer = CreateDefaultSubobject<UTimerConstraint>(TEXT("DurationTimer"));
	Timer->OnExpire.AddDynamic(this, &UModifierBase::OnExpire);
	Timer->SetDuration(LifetimeDuration);

	ConstraintArray = TArray<UConstraintBase*>({ Timer });
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



void UModifierBase::OnExpire()
{
	OnHasExpired.Broadcast(this);
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

void UModifierBase::Tick(float DeltaTime)
{
	for (auto Constraint : ConstraintArray)
	{
		Constraint->Tick(DeltaTime);
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

void UModifierBase::ClearTimer()
{
	Timer = nullptr;
}


void UModifierBase::Cleanup()
{
	ConditionalBeginDestroy();
	ClearConstraints();
	ClearTimer();
	UE_LOG(LogTemp, Warning, TEXT("Modifier is cleaning up: %s"), *GetName())
}