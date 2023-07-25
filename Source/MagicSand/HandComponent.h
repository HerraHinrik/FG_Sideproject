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


public:
	UFUNCTION(BlueprintImplementableEvent)
		void PlayCard(int32 CardIndex, AFirstPersonViewCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent)
		bool AddCardsToEmptySlots(const TArray<UCardBase*>& Hand, UCardBase* Card);
};
