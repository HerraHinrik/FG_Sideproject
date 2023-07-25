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
	// Sets default values for this component's properties
	UPlayerModifierComponent();

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnApplyModifier;

	UPROPERTY(BlueprintAssignable);
	FPlayerModifierDelegate OnRemoveModifier;

protected:
	UPROPERTY()
	TArray<UPlayerModifier*> ModifierArray;

	UPROPERTY(Replicated)
	FPlayerStatBlock ActiveModifications;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	FPlayerStatBlock MaxModifications;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
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

	UFUNCTION(BlueprintCallable)
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
	void ApplyPlayerModifier(TSubclassOf<UPlayerModifier> Modifier);

	// UI facing interface
	UFUNCTION(BlueprintCallable)
	TArray<FModifierUIData> GetActiveModifierData();
};
