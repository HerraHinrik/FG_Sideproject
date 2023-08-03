// Copyright Epic Games, Inc. All Rights Reserved.

#include "MagicSandGameModeBase.h"
#include "MagicSandPlayerState.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"
#include "TeamSpawnPoint.h"

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
	return 0; //implement later
}