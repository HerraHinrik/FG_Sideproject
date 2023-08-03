// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "TeamSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class MAGICSAND_API ATeamSpawnPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	/** Which team can start at this point */
	UPROPERTY(EditDefaultsOnly, Category = Team)
	int32 SpawnTeam;
	
};
