// By Amos Johan Persson


#include "GunComponent.h"
#include "Loadout.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ULoadout* Shotgun = CreateDefaultSubobject<ULoadout>(TEXT("ShotgunLoadout"));
	ULoadout* BoltAction = CreateDefaultSubobject<ULoadout>(TEXT("BoltActionLoadout"));

	LoadoutArray = TArray<ULoadout*>({ Shotgun, BoltAction });
	SetCurrentLoadoutByIndex(0);
	RegisterComponent();
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

	int Index = CurrentIndex;
	Index ++;
	Index %= LoadoutArray.Num();
	SetCurrentLoadoutByIndex(Index);
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

void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(CurrentLoadout))
	{
		return;
	}

	CurrentLoadout->Tick(DeltaTime, TickType, ThisTickFunction);
}

void UGunComponent::BeginPlay()
{
	//LoadoutArray[0] = BuildShotgunLoadout();
	//LoadoutArray[1] = BuildBoltLoadout();
}