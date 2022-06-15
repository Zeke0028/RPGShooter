// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/CharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerControllerGameplay.h"
#include "Player/PlayerStateGameplay.h"
#include "Characters/Components/PawnPlayerComponent.h"

FName const ACharacterPlayer::NAME_PawnPlayerComponent(TEXT("PawnPlayerComponent"));

ACharacterPlayer::ACharacterPlayer()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	PawnPlayerComponent = CreateDefaultSubobject<UPawnPlayerComponent>(NAME_PawnPlayerComponent);

}

APlayerControllerGameplay* ACharacterPlayer::GetPlayerControllerGameplay() const
{
	return CastChecked<APlayerControllerGameplay>(GetController(), ECastCheckedType::NullAllowed);
}

APlayerStateGameplay* ACharacterPlayer::GetPlayerStateGameplay() const
{
	return CastChecked<APlayerStateGameplay>(GetPlayerState(), ECastCheckedType::NullAllowed);
}
