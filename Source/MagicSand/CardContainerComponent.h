// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Character.h"
#include "CardBase.h"
#include "Components/Widget.h"
#include "CardContainerComponent.generated.h"


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGICSAND_API UCardContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCardContainerComponent();

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<UCardBase*> Cards;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<UCardBase*> Discard;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UWidget* HUDWidget;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddCard(TSubclassOf<UCardBase> Card);

	UFUNCTION(BlueprintCallable)
	void AddCardToHand(UCardBase* Card);

	UFUNCTION(BlueprintCallable)
	void RemoveCard(UCardBase* Card);

	UFUNCTION(BlueprintCallable)
	ACharacter* GetPlayer();

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeCardComponent();
};
