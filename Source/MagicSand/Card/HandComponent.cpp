// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"

void UHandComponent::ServerPlayCard_Implementation(int32 CardIndex, AFirstPersonViewCharacter* Target)
{
	PlayCard(CardIndex, Target);
}

bool UHandComponent::ServerPlayCard_Validate(int32 CardIndex, AFirstPersonViewCharacter* Target)
{
	return true;
}