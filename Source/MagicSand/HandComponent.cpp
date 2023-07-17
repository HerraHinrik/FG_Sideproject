// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"


void UHandComponent::AddMultiple(TArray<UCardBase*> NewCards)
{
	for (UCardBase* Card : NewCards)
	{
		AddCard(Card);
	}
}