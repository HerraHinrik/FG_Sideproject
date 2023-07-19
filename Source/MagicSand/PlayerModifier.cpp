// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerModifier.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerModifier::TickModifier(float DeltaTime)
{
	ExpiredDuration += DeltaTime;

	if (ExpiredDuration >= LifetimeDuration)
	{
		OnExpire.Broadcast(this);
	}
}

FPlayerStatBlock UPlayerModifier::GetStatModifications()
{
	return StatModifications;
}


int32 UPlayerModifier::GetIconID()
{
	return IconID;
}

float UPlayerModifier::GetDurationLeft()
{
	float TimeLeft = LifetimeDuration - ExpiredDuration;
	return UKismetMathLibrary::Max(0, TimeLeft);
}

float UPlayerModifier::GetDurationMax()
{
	return LifetimeDuration;
}