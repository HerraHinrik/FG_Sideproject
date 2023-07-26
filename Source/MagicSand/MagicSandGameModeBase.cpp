// Copyright Epic Games, Inc. All Rights Reserved.


#include "MagicSandGameModeBase.h"
#include "FirstPersonViewCharacter.h"

void AMagicSandGameModeBase::BeginPlay()
{
    Super::BeginPlay();

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