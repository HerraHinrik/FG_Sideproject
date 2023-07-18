// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardContainerComponent.h"
#include "HandComponent.generated.h"

/**
 * 
 */
UCLASS()
class MAGICSAND_API UHandComponent : public UCardContainerComponent
{
	GENERATED_BODY()
	

protected:
	UPROPERTY()
	TArray<int32> CardIDs;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayCard(AFirstPersonViewCharacter* Target);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool IsFull();

	UFUNCTION(BlueprintCallable)
		void AddMultiple(TArray<UCardBase*> &NewCards);
};
