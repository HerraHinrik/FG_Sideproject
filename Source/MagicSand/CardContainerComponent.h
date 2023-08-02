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
	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly)
	TArray<UCardBase*> Cards;

	UPROPERTY(Replicated, BlueprintReadWrite, EditDefaultsOnly)
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

	UFUNCTION(Server, reliable, WithValidation, BlueprintCallable)
	void AddCard(TSubclassOf<UCardBase> Card);

	UFUNCTION(BlueprintCallable)
	ACharacter* GetPlayer();

	UFUNCTION()
	void MakeNetAddressable();

	UFUNCTION(Server, reliable, WithValidation)
	void ServerInitializeCardComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeCardComponent();
};
