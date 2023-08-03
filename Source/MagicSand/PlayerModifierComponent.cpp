// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerModifierComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "FirstPersonViewCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerModifierComponent::UPlayerModifierComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponent();

	SetNetAddressable();
	SetIsReplicated(true);
}

// Called when the game starts
void UPlayerModifierComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UPlayerModifierComponent::OnTakeDamage);

	ActiveModifications.Health = MaxModifications.Health;
}

void UPlayerModifierComponent::ApplyModifications_Implementation(FPlayerStatBlock StatChanges)
{
	ActiveModifications.Armor += StatChanges.Armor;
	ActiveModifications.DamageFlat += StatChanges.DamageFlat;
	ActiveModifications.DamageMultiplier += StatChanges.DamageMultiplier;
	ActiveModifications.SpeedMultiplier += StatChanges.SpeedMultiplier;
	ActiveModifications.Health += StatChanges.Health;

	if (ActiveModifications.Health <= MinModifications.Health)
	{
		auto Character = Cast<AFirstPersonViewCharacter>(GetOwner());

		if (!IsValid(Character))
		{
			return;
		}

		Character->CallRestartPlayer();
	}
}

bool UPlayerModifierComponent::ApplyModifications_Validate(FPlayerStatBlock StatChanges)
{
	return true;
}

void UPlayerModifierComponent::CleanUpModifications_Implementation(FPlayerStatBlock StatChanges)
{
	ActiveModifications.Armor -= StatChanges.Armor;
	ActiveModifications.DamageFlat -= StatChanges.DamageFlat;
	ActiveModifications.DamageMultiplier -= StatChanges.DamageMultiplier;
	ActiveModifications.SpeedMultiplier -= StatChanges.SpeedMultiplier;
	ActiveModifications.Health -= StatChanges.Health;
}

bool UPlayerModifierComponent::CleanUpModifications_Validate(FPlayerStatBlock StatChanges)
{
	return true;
}

void UPlayerModifierComponent::RemovePlayerModifier(UPlayerModifier* Modifier)
{
	FPlayerStatBlock StatChanges = Modifier->GetStatModifications();
	CleanUpModifications(StatChanges);
	ModifierArray.Remove(Modifier);
	BroadcastModifierDelegate(OnRemoveModifier, Modifier);
}

void UPlayerModifierComponent::OnTakeDamage_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FPlayerStatBlock HealthChange;
	HealthChange.Health = Damage;

	ApplyModifications(HealthChange);
	BroadcastModifierDelegate(OnApplyModifier, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("%s took damage"), *DamagedActor->GetName())
	UE_LOG(LogTemp, Warning, TEXT("%f health left"), GetCurrentModifications().Health)
}

bool UPlayerModifierComponent::OnTakeDamage_Validate(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	return true;
}

// Called every frame
void UPlayerModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	for (int i = 0; i < ModifierArray.Num(); i++)
	{
		if (!IsValid(ModifierArray[i]))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found invalid modifier in PlayerModifierComponent"))
			continue;
		}

		if (!ModifierDurations.IsValidIndex(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found invalid duration index in PlayerModifierComponent"))
			continue;
		}

		ModifierDurations[i] += DeltaTime;

		if (ModifierDurations[i] >= ModifierArray[i]->GetDurationMax())
		{
			UE_LOG(LogTemp, Warning, TEXT("Expired player modifier is added to list"))
			ExpiredModifiers.Add(ModifierArray[i]);
		}
	}

	for (UPlayerModifier* Modifier : ExpiredModifiers)
	{
		int32 Index = ModifierArray.IndexOfByKey(Modifier);
		ModifierDurations.RemoveAt(Index);

		RemovePlayerModifier(Modifier);
	}

	ExpiredModifiers.Empty();
}

void UPlayerModifierComponent::BroadcastModifierDelegate_Implementation(const FPlayerModifierDelegate& Event, UPlayerModifier* Modifier)
{
	Event.Broadcast(Modifier);
}

FPlayerStatBlock UPlayerModifierComponent::GetCurrentModifications()
{
	FPlayerStatBlock ValidatedModifications = FPlayerStatBlock();

	ValidatedModifications.Armor = UKismetMathLibrary::Clamp(ActiveModifications.Armor, MinModifications.Armor, MaxModifications.Armor);
	ValidatedModifications.DamageFlat = UKismetMathLibrary::Clamp(ActiveModifications.DamageFlat, MinModifications.DamageFlat, MaxModifications.DamageFlat);
	ValidatedModifications.DamageMultiplier = UKismetMathLibrary::Clamp(ActiveModifications.DamageMultiplier, MinModifications.DamageMultiplier, MaxModifications.DamageMultiplier);
	ValidatedModifications.SpeedMultiplier = UKismetMathLibrary::Clamp(ActiveModifications.SpeedMultiplier, MinModifications.SpeedMultiplier, MaxModifications.SpeedMultiplier);

	ValidatedModifications.Health = UKismetMathLibrary::Max(ActiveModifications.Health, MinModifications.Health);

	return ValidatedModifications;
}

void UPlayerModifierComponent::ApplyPlayerModifier(TSubclassOf<UPlayerModifier> ModifierClass)
{
	FName UniqueName = MakeUniqueObjectName(this, UPlayerModifier::StaticClass());
	UPlayerModifier* NewModifier = NewObject<UPlayerModifier>(this, ModifierClass, UniqueName);
	NewModifier->InitializeStatValues();

	if (NewModifier->ResetsOnExpire())
	{
		ModifierArray.Add(NewModifier);
		ModifierDurations.Add(0);
	}

	ApplyModifications(NewModifier->GetStatModifications());
	BroadcastModifierDelegate(OnApplyModifier, NewModifier);
}

TArray<FModifierUIData> UPlayerModifierComponent::GetActiveModifierData()
{
	TArray<FModifierUIData> Results;

	for (UPlayerModifier* Modifier : ModifierArray)
	{
		if (!IsValid(Modifier))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid modifier pointer while requesting UI data"))
				continue;
		}

		FModifierUIData Info = FModifierUIData();
		Info.IconID = Modifier->GetIconID();
		Info.DurationLeft = Modifier->GetDurationLeft();
		Info.DurationMax = Modifier->GetDurationMax();
		Results.Add(Info);
	}

	return Results;
}

void UPlayerModifierComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(UPlayerModifierComponent, ActiveModifications);
	DOREPLIFETIME(UPlayerModifierComponent, MaxModifications);
	DOREPLIFETIME(UPlayerModifierComponent, MinModifications);
}