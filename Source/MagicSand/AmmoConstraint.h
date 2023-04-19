// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConstraintBase.h"
#include "AmmoConstraint.generated.h"


/**
 * 
 */
UCLASS()
class UAmmoConstraint : public UConstraintBase
{
	GENERATED_BODY()

	UAmmoConstraint();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxAmmo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int CurrentAmmo;

public:

public:
	virtual void BeginPlay();

	// Inherited
	virtual bool Evaluate() const override;

	virtual void ProcessFire() override;

	virtual void ResponseFunction() override;
	// End Inherited

	UFUNCTION(BlueprintCallable)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable)
	int GetMaxAmmo() const;

	UFUNCTION(BlueprintCallable)
	void SetCurrentAmmo(int Value);


	UFUNCTION(BlueprintCallable)
	void SetMaxAmmo(int Value);

};
