// Copyright Epic Games, Inc. All Rights Reserved.


#include "MagicSandGameModeBase.h"

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