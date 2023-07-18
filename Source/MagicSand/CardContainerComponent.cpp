// Fill out your copyright notice in the Description page of Project Settings.


#include "CardContainerComponent.h"

// Sets default values for this component's properties
UCardContainerComponent::UCardContainerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardContainerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCardContainerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCardContainerComponent::AddCard(TSubclassOf<UCardBase> &Card)
{
	auto NewCard = NewObject<UCardBase>(this, Card);
	Cards.Add(NewCard);
}

void UCardContainerComponent::AddCardToHand(UCardBase* Card)
{
	Cards.Add(Card);
}

void UCardContainerComponent::RemoveCard(UCardBase* Card)
{
	Cards.Remove(Card);
}

