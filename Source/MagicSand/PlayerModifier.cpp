// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerModifier.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerModifierOld::TickModifier(float DeltaTime)
{
	ExpiredDuration += DeltaTime;

	if (ExpiredDuration >= LifetimeDuration)
	{
		OnExpire.Broadcast(this);
	}
}

FPlayerStatBlock UPlayerModifierOld::GetStatModifications()
{
	return StatModifications;
}


int32 UPlayerModifierOld::GetIconID()
{
	return IconID;
}

float UPlayerModifierOld::GetDurationLeft()
{
	float TimeLeft = LifetimeDuration - ExpiredDuration;
	return UKismetMathLibrary::Max(0, TimeLeft);
}

float UPlayerModifierOld::GetDurationMax()
{
	return LifetimeDuration;
}