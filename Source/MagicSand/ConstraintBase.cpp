// By Amos Johan Persson


#include "ConstraintBase.h"

UConstraintBase::UConstraintBase()
{
	bUsesTick = GIsRunning;
}

bool UConstraintBase::Evaluate_Implementation()
{
	return true;
}

void UConstraintBase::ProcessFire_Implementation()
{
	
}

void UConstraintBase::OnReload_Implementation()
{

}

void UConstraintBase::ConstraintTick(float DeltaTime)
{
	LastTickFrame = GFrameCounter;
}

bool UConstraintBase::CanTick()
{
	return bUsesTick && (LastTickFrame != GFrameCounter);
}

