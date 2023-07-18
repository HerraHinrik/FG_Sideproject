// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentLoadout = nullptr;
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	ULoadout* Shotgun = NewObject<ULoadout>(this, TEXT("Shotgun"));
	ULoadout* BoltAction = NewObject<ULoadout>(this, TEXT("BoltAction"));

	BuildBoltLoadout(BoltAction);
	BuildShotgunLoadout(Shotgun);

	AddLoadout(BoltAction);
	AddLoadout(Shotgun);
}

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	for (ULoadout* Loadout : LoadoutArray)
	{ 
		if (!IsValid(Loadout)) continue;

		Loadout->TickLoadout(DeltaTime);
	}
}

void UGunComponent::RegisterReloadSubscribers(ULoadout* Loadout)
{
	auto ConstraintsArray = Loadout->GetConstraints();
	for (auto Constraint : ConstraintsArray)
	{
		// Empty response function on ConstraintBase allows children to add custom reload response
		OnReload.AddDynamic(Constraint, &UConstraintBase::OnReload);
	}
}

void UGunComponent::ClearReloadSubscribers()
{
	OnReload.Clear();
}

void UGunComponent::SetCurrentLoadoutByIndex(uint32 Index)
{
	CurrentLoadout = LoadoutArray[Index];
	CurrentIndex = Index;
}

void UGunComponent::ToggleLoadout()
{
	if (LoadoutArray.IsEmpty())
	{
		return;
	}

	ClearReloadSubscribers();

	uint32 Index = CurrentIndex;
	Index ++;
	Index %= LoadoutArray.Num();

	SetCurrentLoadoutByIndex(Index);
	RegisterReloadSubscribers(CurrentLoadout);
}


void UGunComponent::AddLoadout(ULoadout* Loadout)
{
	ClearReloadSubscribers();
	RegisterReloadSubscribers(Loadout);
	Reload();

	uint32 Index = LoadoutArray.Add(Loadout);
	SetCurrentLoadoutByIndex(Index);

}

void UGunComponent::ApplyModifier(TSubclassOf<UModifierBase> &Modifier)
{
	CurrentLoadout->AddModifier(Modifier);
}

void UGunComponent::Fire()
{
	UCameraComponent* origin = GetOwner()->FindComponentByClass<UCameraComponent>();

	if (!IsValid(origin) || !IsValid(CurrentLoadout)) return;

	FRotator Rotation = origin->GetComponentRotation();
	FVector Location = origin->GetComponentLocation();

	CurrentLoadout->Fire(Location, Rotation);
}

void UGunComponent::Reload()
{
	OnReload.Broadcast();
}