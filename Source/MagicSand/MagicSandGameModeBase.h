// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstPersonViewCharacter.h"
#include "MagicSandGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

/**
 * 
 */
UCLASS()
class MAGICSAND_API AMagicSandGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

protected:
    UPROPERTY()
    FOnPlayerDiedSignature OnPlayerDied;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void PlayerDied(ACharacter* Character);

public:
    virtual void RestartPlayer(AController* NewPlayer) override;

    const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }
	
};
