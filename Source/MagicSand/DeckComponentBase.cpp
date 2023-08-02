// Fill out your copyright notice in the Description page of Project Settings.


#include "DeckComponentBase.h"

void UDeckComponentBase::ServerReshuffle_Implementation()
{
	Reshuffle();
}


bool UDeckComponentBase::ServerReshuffle_Validate()
{
	return true;
}

void UDeckComponentBase::ServerDrawCards_Implementation(int32 Amount)
{

}

bool UDeckComponentBase::ServerDrawCards_Validate(int32 Amount)
{
	return true;
}


void UDeckComponentBase::ServerRepopulateDeck_Implementation()
{
	RepopulateDeck();
}

bool UDeckComponentBase::ServerRepopulateDeck_Validate()
{
	return true;
}