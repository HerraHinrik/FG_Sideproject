// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardContainerComponent.h"
#include "DeckComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class MAGICSAND_API UDeckComponentBase : public UCardContainerComponent
{
	GENERATED_BODY()
	
protected:

public:

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Reshuffle();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	TArray<UCardBase*> DrawCards(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RepopulateDeck();
};
