// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConstraintBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class UConstraintBase : public UObject
{
	GENERATED_BODY()
	

protected:
	TMap<FName, float> InternalValues;

	UPROPERTY(BlueprintReadOnly)
	bool bUsesTick;

public:
	UFUNCTION(BlueprintCallable)
	float GetInternalValue(FName Name) const;

	// Overrride these to implement logic
	UFUNCTION(BlueprintCallable)
	virtual bool Evaluate() const;

	UFUNCTION(BlueprintCallable)
	virtual void ProcessFire();

	UFUNCTION(BlueprintCallable)
	virtual void OnReload();

	virtual void Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
};
