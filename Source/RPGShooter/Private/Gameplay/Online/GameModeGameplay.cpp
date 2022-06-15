// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Online/GameModeGameplay.h"
#include "Gameplay/Player/PlayerControllerGameplay.h"
#include "Gameplay/Player/PlayerStateGameplay.h"
#include "Gameplay/Characters/CharacterPlayer.h"


AGameModeGameplay::AGameModeGameplay()
{
	PlayerControllerClass = APlayerControllerGameplay::StaticClass();
	PlayerStateClass = APlayerStateGameplay::StaticClass();
	DefaultPawnClass = ACharacterPlayer::StaticClass();
}
