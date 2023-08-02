// Fill out your copyright notice in the Description page of Project Settings.


#include "CardContainerComponent.h"
#include "Net/UnrealNetwork.h"

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

void UCardContainerComponent::AddCard_Implementation(TSubclassOf<UCardBase> Card)
{
	auto NewCard = NewObject<UCardBase>(this, Card);
	Cards.Add(NewCard);
}

bool UCardContainerComponent::AddCard_Validate(TSubclassOf<UCardBase> Card)
{
	return true;
}

ACharacter* UCardContainerComponent::GetPlayer()
{
	UWorld* const World = GetWorld();
	APlayerController* Controller = World->GetFirstPlayerController();

	if (!IsValid(Controller)) return nullptr;


	ACharacter* Character = Controller->GetCharacter();

	return Character;
}

void UCardContainerComponent::MakeNetAddressable()
{
	SetNetAddressable();
	SetIsReplicated(true);
}

void UCardContainerComponent::ServerInitializeCardComponent_Implementation()
{
	InitializeCardComponent();
}

bool UCardContainerComponent::ServerInitializeCardComponent_Validate()
{
	return true;
}

void UCardContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(UCardContainerComponent, Cards);
	DOREPLIFETIME(UCardContainerComponent, Discard);

}