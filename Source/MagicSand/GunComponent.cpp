// Fill out your copyright notice in the Description page of Project Settings.


#include "GunComponent.h"
#include "Loadout.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Make empty current loadout
	CurrentLoadout = CreateDefaultSubobject<ULoadout>(FName(TEXT("DefaultLoadout")));
}


// Called when the game starts
void UGunComponent::RegisterReloadSubscribers(ULoadout* Loadout)
{
	auto ConstraintsArray = Loadout->GetConstraints();
	for (auto Constraint : ConstraintsArray)
	{
		// Empty response function on ConstraintBase allows children to add custom reload response
		OnReload.AddDynamic(Constraint, &UConstraintBase::OnReload);
	}
}

void UGunComponent::SetCurrentLoadout(int Index)
{
	CurrentLoadout = LoadoutArray[Index];
}


void UGunComponent::AddLoadout(ULoadout* Loadout)
{
	int32 Index = LoadoutArray.Add(Loadout);
	RegisterReloadSubscribers(Loadout);
	SetCurrentLoadout(Index);
}

// Called every frame
void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentLoadout)) 
	{
		return; 
	}

	TArray<UConstraintBase*> ConstraintsArray = CurrentLoadout->GetConstraints();

	// Cooldowns need tick propagated to them
	for (UConstraintBase* Constraint : ConstraintsArray)
	{
		Constraint->Tick(DeltaTime, TickType, ThisTickFunction);
	}
}

void UGunComponent::Fire()
{
	// Get gun part arrays
	TArray<UConstraintBase*> Constraints = CurrentLoadout->GetConstraints();
	TArray<USpawnerBase*> Spawners = CurrentLoadout->GetSpawners();
	TArray<UModifierBase*> Modifiers = CurrentLoadout->GetModifiers();

	// Constraints
	for (UConstraintBase* Constraint : Constraints)
	{
		if (Constraint == nullptr) continue;

		if (!Constraint->Evaluate())
		{
			UE_LOG(LogTemp, Warning, TEXT("This constraint failed: %s"), *Constraint->GetName())
			return;
		}
	}

	// Update constraints internal logic
	for (UConstraintBase* Constraint : Constraints)
	{
		if (Constraint == nullptr) continue;

		UE_LOG(LogTemp, Warning, TEXT("This constraint is processing: %s"), *Constraint->GetName())
		Constraint->ProcessFire();

	}

	// Spawning
	TArray<AActor*> NewProjectiles = TArray<AActor*>();
	FRotator Rotation = GetOwner()->GetActorRotation();
	FVector Location = GetOwner()->GetActorLocation();

	for (USpawnerBase* Spawner : Spawners)
	{
		NewProjectiles.Append(Spawner->SpawnProjectiles(Location, Rotation));
	}

	// Modifiers
	for (UModifierBase* Modifier : Modifiers)
	{
		Modifier->ProcessProjectiles(NewProjectiles);
	}
}

void UGunComponent::Reload()
{
	OnReload.Broadcast();
}