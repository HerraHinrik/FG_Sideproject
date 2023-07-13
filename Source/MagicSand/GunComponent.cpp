// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ULoadout* Shotgun = CreateDefaultSubobject<ULoadout>(TEXT("Shotgun"));
	ULoadout* BoltAction = CreateDefaultSubobject<ULoadout>(TEXT("BoltAction"));

	BuildBoltLoadout(BoltAction);
	BuildShotgunLoadout(Shotgun);

	LoadoutArray = TArray<ULoadout*>({ Shotgun, BoltAction });
	SetCurrentLoadoutByIndex(0);
	RegisterReloadSubscribers(CurrentLoadout);
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

void UGunComponent::SetCurrentLoadoutByIndex(int Index)
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

	int Index = CurrentIndex;
	Index ++;
	Index %= LoadoutArray.Num();

	SetCurrentLoadoutByIndex(Index);
	RegisterReloadSubscribers(CurrentLoadout);
}


void UGunComponent::AddLoadout(ULoadout* Loadout)
{
	int32 Index = LoadoutArray.Add(Loadout);
	RegisterReloadSubscribers(Loadout);
	SetCurrentLoadoutByIndex(Index);
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

void UGunComponent::CustomTick(float DeltaTime)
{

	if (!IsValid(CurrentLoadout))
	{
		return;
	}

	CurrentLoadout->Tick(DeltaTime);
}