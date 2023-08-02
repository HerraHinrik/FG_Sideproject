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
	UFUNCTION(Server, reliable, WithValidation, BlueprintCallable)
	void ServerReshuffle();

	UFUNCTION(Server, reliable, WithValidation, BlueprintCallable)
	void ServerDrawCards(int32 Amount);

	UFUNCTION(Server, reliable, WithValidation, BlueprintCallable)
	void ServerRepopulateDeck();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Reshuffle();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	TArray<UCardBase*> DrawCards(int32 Amount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void RepopulateDeck();
};
