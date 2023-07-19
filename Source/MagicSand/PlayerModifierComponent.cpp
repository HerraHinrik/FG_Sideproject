// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerModifierComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPlayerModifierComponent::UPlayerModifierComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RegisterComponent();

	ActiveModifications.Armor = 0;
	ActiveModifications.DamageFlat = 0;
	ActiveModifications.DamageMultiplier = 0;
	ActiveModifications.SpeedMultiplier = 0;
	ActiveModifications.Health = MaxModifications.Health;
}


// Called when the game starts
void UPlayerModifierComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UPlayerModifierComponent::OnTakeDamage);
	
}

void UPlayerModifierComponent::ApplyModifications(FPlayerStatBlock StatChanges)
{
	ActiveModifications.Armor += StatChanges.Armor;
	ActiveModifications.DamageFlat += StatChanges.DamageFlat;
	ActiveModifications.DamageMultiplier += StatChanges.DamageMultiplier;
	ActiveModifications.SpeedMultiplier += StatChanges.SpeedMultiplier;
	ActiveModifications.Health += StatChanges.Health;
}


void UPlayerModifierComponent::CleanUpModifications(FPlayerStatBlock StatChanges)
{
	ActiveModifications.Armor -= StatChanges.Armor;
	ActiveModifications.DamageFlat -= StatChanges.DamageFlat;
	ActiveModifications.DamageMultiplier -= StatChanges.DamageMultiplier;
	ActiveModifications.SpeedMultiplier -= StatChanges.SpeedMultiplier;
	ActiveModifications.Health -= StatChanges.Health;
}

void UPlayerModifierComponent::RemovePlayerModifier(UPlayerModifier* Modifier)
{
	FPlayerStatBlock StatChanges = Modifier->GetStatModifications();
	CleanUpModifications(StatChanges);
	ModifierArray.Remove(Modifier);
}

void UPlayerModifierComponent::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	FPlayerStatBlock HealthChange;
	HealthChange.Health = Damage;

	ApplyModifications(HealthChange);
}

// Called every frame
void UPlayerModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (UPlayerModifier* Modifier : ModifierArray)
	{
		if (!IsValid(Modifier))
		{
			UE_LOG(LogTemp, Warning, TEXT("Found invalid player modifier during tick."))
			continue;
		}

		Modifier->TickModifier(DeltaTime);
	}
}


FPlayerStatBlock UPlayerModifierComponent::GetCurrentModifications()
{
	FPlayerStatBlock ValidatedModifications = FPlayerStatBlock();
	ValidatedModifications.Armor = UKismetMathLibrary::Clamp(ActiveModifications.Armor, MinModifications.Armor, MaxModifications.Armor);
	ValidatedModifications.DamageFlat = UKismetMathLibrary::Clamp(ActiveModifications.DamageFlat, MinModifications.DamageFlat, MaxModifications.DamageFlat);
	ValidatedModifications.DamageMultiplier = UKismetMathLibrary::Clamp(ActiveModifications.DamageMultiplier, MinModifications.DamageMultiplier, MaxModifications.DamageMultiplier);
	ValidatedModifications.SpeedMultiplier = UKismetMathLibrary::Clamp(ActiveModifications.SpeedMultiplier, MinModifications.SpeedMultiplier, MaxModifications.SpeedMultiplier);
	ValidatedModifications.Health = UKismetMathLibrary::Clamp(ActiveModifications.Health, MinModifications.Health, MaxModifications.Health);

	return ValidatedModifications;
}

void UPlayerModifierComponent::ApplyPlayerModifier(UPlayerModifier* Modifier)
{
	FName UniqueName = MakeUniqueObjectName(this, UPlayerModifier::StaticClass());
	UPlayerModifier* NewModifier = DuplicateObject<UPlayerModifier>(Modifier, this, UniqueName);
	ModifierArray.Add(NewModifier);

	ApplyModifications(Modifier->GetStatModifications());
	NewModifier->OnExpire.AddDynamic(this, &UPlayerModifierComponent::RemovePlayerModifier);
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