// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "FirstPersonViewCharacter.h"
#include "MagicSandGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

/**
 * 
 */
UCLASS()
class MAGICSAND_API AMagicSandGameModeBase : public AGameMode
{
    GENERATED_UCLASS_BODY()

public:

protected:
    UPROPERTY()
    FOnPlayerDiedSignature OnPlayerDied;

    /** number of teams */
    UPROPERTY()
    int32 NumTeams;

    /** best team */
    UPROPERTY()
    int32 WinnerTeam;

protected:
    /** setup team changes at player login */
    void PostLogin(APlayerController* NewPlayer) override;

    /** select best spawn point for player */
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void PlayerDied(ACharacter* Character);

    /** check team constraints */
    virtual bool IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Player) const;

    /** pick team with least players in or random when it's equal */
    int32 ChooseTeam(APlayerState* ForPlayerState) const;

public:
    virtual void RestartPlayer(AController* NewPlayer) override;

    const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }
	
};
