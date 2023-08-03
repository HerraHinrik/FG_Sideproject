// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicSandPlayerState.h"
#include "Net/UnrealNetwork.h"

void AMagicSandPlayerState::SetTeamNum(int32 NewTeamNumber)
{
	TeamNumber = NewTeamNumber;
}

void AMagicSandPlayerState::ScoreKill(AMagicSandPlayerState* Victim, int32 Points)
{
	NumKills += 1;
}

void AMagicSandPlayerState::ScoreDeath(AMagicSandPlayerState* KilledBy, int32 Points)
{
	NumDeaths += 1;
}


int32 AMagicSandPlayerState::GetTeamNum() const
{
	return TeamNumber;
}

int32 AMagicSandPlayerState::GetKills() const
{
	return NumKills;
}

int32 AMagicSandPlayerState::GetDeaths() const
{
	return NumDeaths;
}

void AMagicSandPlayerState::UpdateTeamColors()
{
	GetPlayerController()->GetCharacter(); // apply team colors on character
}

bool AMagicSandPlayerState::ShareTeamWith(AMagicSandPlayerState* Other)
{
	return Other->TeamNumber == TeamNumber;
}

void AMagicSandPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMagicSandPlayerState, TeamNumber);
	DOREPLIFETIME(AMagicSandPlayerState, NumKills);
	DOREPLIFETIME(AMagicSandPlayerState, NumDeaths);
}