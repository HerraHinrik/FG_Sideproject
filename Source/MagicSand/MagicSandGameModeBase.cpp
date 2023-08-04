// Copyright Epic Games, Inc. All Rights Reserved.

#include "MagicSandGameModeBase.h"
#include "MagicSandPlayerState.h"
#include "GameFramework/GameState.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"
#include "TeamSpawnPoint.h"

AMagicSandGameModeBase::AMagicSandGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NumTeams = 2;
	//bDelayedStart = true;
}
void AMagicSandGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	// Place player on a team before Super (VoIP team based init, findplayerstart, etc)
	AMagicSandPlayerState* NewPlayerState = CastChecked<AMagicSandPlayerState>(NewPlayer->PlayerState);
	const int32 TeamNum = ChooseTeam(NewPlayerState);
	NewPlayerState->SetTeamNum(TeamNum);

	UE_LOG(LogTemp, Warning, TEXT("Player team: %d"), TeamNum)

	Super::PostLogin(NewPlayer);
}

AActor* AMagicSandGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> FallbackSpawns;

	APlayerStart* BestStart = NULL;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* TestSpawn = *It;
		if (TestSpawn->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			BestStart = TestSpawn;
			break;
		}
		else
		{
			if (IsSpawnpointAllowed(TestSpawn, Player))
			{
				FallbackSpawns.Add(TestSpawn);
			}
		}
	}

	if (BestStart == NULL)
	{
		if (FallbackSpawns.Num() > 0)
		{
			BestStart = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
		}
	}

	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

void AMagicSandGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    //Bind our Player died delegate to the Gamemode's PlayerDied function.
    if (!OnPlayerDied.IsBound())
    {
        OnPlayerDied.AddDynamic(this, &AMagicSandGameModeBase::PlayerDied);
    }
}

void AMagicSandGameModeBase::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
}

void AMagicSandGameModeBase::PlayerDied(ACharacter* Character)
{
    //Get a reference to our Character's Player Controller
    AController* CharacterController = Character->GetController();
    RestartPlayer(CharacterController);
}

bool AMagicSandGameModeBase::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const
{
	if (Player)
	{
		ATeamSpawnPoint* TeamStart = Cast<ATeamSpawnPoint>(SpawnPoint);
		AMagicSandPlayerState* PlayerState = Cast<AMagicSandPlayerState>(Player->PlayerState);

		if (PlayerState && TeamStart && TeamStart->SpawnTeam != PlayerState->GetTeamNum())
		{
			return false;
		}

		return true;
	}

	return false;
}

int32 AMagicSandGameModeBase::ChooseTeam(APlayerState* ForPlayerState) const
{
	TArray<int32> TeamBalance;
	TeamBalance.AddZeroed(NumTeams);

	// get current team balance
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		AMagicSandPlayerState const* const TestPlayerState = Cast<AMagicSandPlayerState>(GameState->PlayerArray[i]);
		if (TestPlayerState && TestPlayerState != ForPlayerState && TeamBalance.IsValidIndex(TestPlayerState->GetTeamNum()))
		{
			TeamBalance[TestPlayerState->GetTeamNum()]++;
		}
	}

	// find least populated one
	int32 BestTeamScore = TeamBalance[0];
	for (int32 i = 1; i < TeamBalance.Num(); i++)
	{
		if (BestTeamScore > TeamBalance[i])
		{
			BestTeamScore = TeamBalance[i];
		}
	}

	// there could be more than one...
	TArray<int32> BestTeams;
	for (int32 i = 0; i < TeamBalance.Num(); i++)
	{
		if (TeamBalance[i] == BestTeamScore)
		{
			BestTeams.Add(i);
		}
	}

	// get random from best list
	const int32 RandomBestTeam = BestTeams[FMath::RandHelper(BestTeams.Num())];
	return RandomBestTeam;
}