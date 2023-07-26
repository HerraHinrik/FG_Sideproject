// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerModifierComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerModifierComponent::UPlayerModifierComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponent();

	//ActiveModifications.Armor = 0;
	//ActiveModifications.DamageFlat = 0;
	//ActiveModifications.DamageMultiplier = 0;
	//ActiveModifications.SpeedMultiplier = 0;
	//ActiveModifications.Health = 0;

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

void UPlayerModifierComponent::RemovePlayerModifier(FPlayerModifier Modifier)
{
	FPlayerStatBlock StatChanges = Modifier.StatModifications;
	CleanUpModifications(StatChanges);
	ModifierArray.Remove(Modifier);
	OnRemoveModifier.Broadcast(Modifier.StatModifications);
}

void UPlayerModifierComponent::OnTakeDamage_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FPlayerStatBlock HealthChange;
	HealthChange.Health = -Damage;

	ApplyModifications(HealthChange);
	CheckHealth();
}

bool UPlayerModifierComponent::OnTakeDamage_Validate(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	return true;
}

void UPlayerModifierComponent::CheckHealth_Implementation()
{
	if (ActiveModifications.Health >= MinModifications.Health)
	{

		OnOutOfHealth.Broadcast();

	}
}

bool UPlayerModifierComponent::CheckHealth_Validate()
{
	return true;
}

// Called every frame
void UPlayerModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FPlayerModifier Modifier : ModifierArray)
	{

		Modifier.ExpiredLifespan += DeltaTime;

		if (Modifier.ExpiredLifespan >= Modifier.MaxLifespan)
		{
			ExpiredModifiers.Add(Modifier);
		}
	}

	for (FPlayerModifier Modifier : ExpiredModifiers)
	{
		ModifierArray.Remove(Modifier);
	}

	ExpiredModifiers.Empty();
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

void UPlayerModifierComponent::ApplyPlayerModifier(FPlayerModifier Modifier)
{
	// Means modifier will be tracked and removed
	if (Modifier.ResetOnEnd == true) ModifierArray.Add(Modifier);

	ApplyModifications(Modifier.StatModifications);
	OnApplyModifier.Broadcast(Modifier.StatModifications);
}

TArray<FModifierUIData> UPlayerModifierComponent::GetActiveModifierData()
{
	TArray<FModifierUIData> Results;

	for (FPlayerModifier Modifier : ModifierArray)
	{

		FModifierUIData Info = FModifierUIData();
		Info.IconID = Modifier.IconID;
		Info.DurationLeft = Modifier.MaxLifespan - Modifier.ExpiredLifespan;
		Info.DurationMax = Modifier.MaxLifespan;
		Results.Add(Info);
	}

	return Results;
}

void UPlayerModifierComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerModifierComponent, ActiveModifications);
	DOREPLIFETIME(UPlayerModifierComponent, MinModifications);
	DOREPLIFETIME(UPlayerModifierComponent, MaxModifications);
}