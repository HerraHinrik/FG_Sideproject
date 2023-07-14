// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponent();
}

void UGunComponent::BeginPlay()
{
	ULoadout* Shotgun = NewObject<ULoadout>(this, TEXT("Shotgun"));
	ULoadout* BoltAction = NewObject<ULoadout>(this, TEXT("Shotgun"));

	BuildBoltLoadout(BoltAction);
	BuildShotgunLoadout(Shotgun);

	AddLoadout(BoltAction);
	AddLoadout(Shotgun);
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
	ClearReloadSubscribers();
	RegisterReloadSubscribers(Loadout);
	Reload();

	int32 Index = LoadoutArray.Add(Loadout);
	SetCurrentLoadoutByIndex(Index);

}

void UGunComponent::ApplyModifier(TSubclassOf<UModifierBase> Modifier)
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

void UGunComponent::CustomTick(float DeltaTime)
{

	if (!IsValid(CurrentLoadout))
	{
		return;
	}

	CurrentLoadout->Tick(DeltaTime);
}