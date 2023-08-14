// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MagicSandPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MAGICSAND_API AMagicSandPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

protected:
	void UpdateTeamColors();

	/** team number */
	UPROPERTY(Transient, Replicated)
	int32 TeamNumber;

	/** number of kills */
	UPROPERTY(Transient, Replicated)
	int32 NumKills;

	/** number of deaths */
	UPROPERTY(Transient, Replicated)
	int32 NumDeaths;

public:
	UFUNCTION(BlueprintCallable)
	bool ShareTeamWith(AMagicSandPlayerState* Other);

	UFUNCTION(BlueprintCallable)
	void SetTeamNum(int32 NewTeamNumber);

	/** player killed someone */
	void ScoreKill(AMagicSandPlayerState* Victim, int32 Points);

	/** player died */
	void ScoreDeath(AMagicSandPlayerState* KilledBy, int32 Points);

	/** get current team */
	UFUNCTION(BlueprintCallable)
	int32 GetTeamNum() const;

	/** get number of kills */
	int32 GetKills() const;

	/** get number of deaths */
	int32 GetDeaths() const;
};
