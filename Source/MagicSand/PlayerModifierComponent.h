// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerModifier.h"
#include "Components/Widget.h"
#include "PlayerModifierComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGICSAND_API UPlayerModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerModifierComponent();

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnApplyModifier;

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnRemoveModifier;

protected:
	// Tracking modifiers
	UPROPERTY()
	TArray<UPlayerModifier*> ModifierArray;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float> ModifierDurations;

	UPROPERTY()
	TArray<UPlayerModifier*> ExpiredModifiers;

	// Tracking and validating stat changes
	UPROPERTY(Replicated)
	FPlayerStatBlock ActiveModifications;

	UPROPERTY(Replicated, BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock MaxModifications;

	UPROPERTY(Replicated, BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock MinModifications;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, reliable, WithValidation)
	void ApplyModifications(FPlayerStatBlock StatChanges);

	UFUNCTION(Server, reliable, WithValidation)
	void CleanUpModifications(FPlayerStatBlock StatChanges);

	UFUNCTION()
	void RemovePlayerModifier(UPlayerModifier* Modifier);

	UFUNCTION(BlueprintCallable, Server, reliable, WithValidation)
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UWidget* HUDWidget;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FPlayerStatBlock GetCurrentModifications();

	UFUNCTION(BlueprintCallable)
	void ApplyPlayerModifier(TSubclassOf<UPlayerModifier> ModifierClass);

	// UI facing interface
	UFUNCTION(BlueprintCallable)
	TArray<FModifierUIData> GetActiveModifierData();
};
