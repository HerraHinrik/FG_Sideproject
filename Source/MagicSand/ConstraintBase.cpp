// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstraintBase.h"

float UConstraintBase::GetInternalValue(FName Name) const
{
	if (InternalValues.Contains(Name))
	{
		return InternalValues[Name];
	}
	else
	{
		return -1.0;
	}
}

bool UConstraintBase::Evaluate() const
{
	return true;
}

void UConstraintBase::ProcessFire()
{
	
}

void UConstraintBase::OnReload()
{

}

void UConstraintBase::Tick(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}