// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Widget.h"
#include "ModifierStructs.h"
#include "PlayerModifierComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerModifierDelegate, FPlayerStatBlock, Modifier);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGICSAND_API UPlayerModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerModifierComponent();

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnApplyModifier;

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnRemoveModifier;

protected:
	UPROPERTY()
	TArray<FPlayerModifier> ModifierArray;

	UPROPERTY()
	TArray<FPlayerModifier> ExpiredModifiers;

	UPROPERTY(Replicated)
	FPlayerStatBlock ActiveModifications;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock MaxModifications;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock MinModifications;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, reliable, WithValidation)
	void ApplyModifications(FPlayerStatBlock StatChanges);

	UFUNCTION(Server, reliable, WithValidation)
	void CleanUpModifications(FPlayerStatBlock StatChanges);

	UFUNCTION()
	void RemovePlayerModifier(FPlayerModifier Modifier);

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
	void ApplyPlayerModifier(FPlayerModifier Modifier);

	// UI facing interface
	UFUNCTION(BlueprintCallable)
	TArray<FModifierUIData> GetActiveModifierData();
};
