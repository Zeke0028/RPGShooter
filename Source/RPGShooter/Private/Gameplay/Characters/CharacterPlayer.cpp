// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/CharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/PlayerControllerGameplay.h"
#include "Player/PlayerStateGameplay.h"
#include "Characters/Components/PawnPlayerComponent.h"
#include "Camera/PlayerCameraComponent.h"

FName const ACharacterPlayer::NAME_PawnPlayerComponent(TEXT("PawnPlayerComponent"));
FName const ACharacterPlayer::NAME_PlayerCameraComponent(TEXT("CameraComponent"));


ACharacterPlayer::ACharacterPlayer()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->MaxAcceleration = 2400.0f;
	GetCharacterMovement()->BrakingFrictionFactor = 1.0f;
	GetCharacterMovement()->BrakingFriction = 6.0f;
	GetCharacterMovement()->GroundFriction = 8.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1400.0f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	PawnPlayerComponent = CreateDefaultSubobject<UPawnPlayerComponent>(NAME_PawnPlayerComponent);

	PlayerCameraComponent = CreateDefaultSubobject<UPlayerCameraComponent>(NAME_PlayerCameraComponent);
	PlayerCameraComponent->SetupAttachment(GetRootComponent());
	PlayerCameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
}

APlayerControllerGameplay* ACharacterPlayer::GetPlayerControllerGameplay() const
{
	return CastChecked<APlayerControllerGameplay>(GetController(), ECastCheckedType::NullAllowed);
}

APlayerStateGameplay* ACharacterPlayer::GetPlayerStateGameplay() const
{
	return CastChecked<APlayerStateGameplay>(GetPlayerState(), ECastCheckedType::NullAllowed);
}
