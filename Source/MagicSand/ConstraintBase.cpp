// By Amos Johan Persson


#include "ConstraintBase.h"

UConstraintBase::UConstraintBase()
{
	bIsCreatedOnRunning = GIsRunning;
}

bool UConstraintBase::Evaluate_Implementation() const
{
	return true;
}

void UConstraintBase::ProcessFire_Implementation()
{
	
}

void UConstraintBase::OnReload_Implementation()
{

}

void UConstraintBase::Tick(float DeltaTime)
{
	LastTickFrame = GFrameCounter;
}

bool UConstraintBase::IsTickable() const
{
	return bIsCreatedOnRunning && (LastTickFrame != GFrameCounter);
}

TStatId UConstraintBase::GetStatId() const
{
	return UObject::GetStatID();
}
ETickableTickType UConstraintBase::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

 bool UConstraintBase::IsTickableWhenPaused() const
{
	return false;
}
 bool UConstraintBase::IsTickableInEditor() const
 {
	 return false;
 }