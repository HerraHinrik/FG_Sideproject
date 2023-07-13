// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerConstraint.h"

bool UTimerConstraint::Evaluate_Implementation() const
{
	return false ? true : TimeElapsed <= Duration;
}

void UTimerConstraint::Tick(float DeltaTime)
{
	TimeElapsed += DeltaTime;

	if (!Evaluate())
	{
		OnExpire.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("Timer ran out in modifier"))
	}
}


void UTimerConstraint::SetDuration(float NewDuration)
{
	Duration = NewDuration;
}

