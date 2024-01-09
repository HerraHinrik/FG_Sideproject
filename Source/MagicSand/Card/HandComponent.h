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
	UPROPERTY(BlueprintReadWrite)
	bool CardAdded;

public:
	UFUNCTION(Server, reliable, WithValidation, BlueprintCallable)
	void ServerPlayCard(int32 CardIndex, AFirstPersonViewCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayCard(int32 CardIndex, AFirstPersonViewCharacter* Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool AddCardsToEmptySlots(const TArray<UCardBase*>& Hand, UCardBase* Card);
};
