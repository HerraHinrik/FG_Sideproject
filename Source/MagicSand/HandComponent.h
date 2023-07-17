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
	TArray<int> CardIDs;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayCard(AFirstPersonViewCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent)
		bool IsFull();

	UFUNCTION(BlueprintCallable)
		void AddMultiple(TArray<UCardBase*> NewCards);
};