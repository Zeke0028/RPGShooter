// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Player/PlayerControllerGameplay.h"
#include "GAS/AbilitySystemCharacterComponent.h"
#include "Player/PlayerStateGameplay.h"

void APlayerControllerGameplay::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UAbilitySystemCharacterComponent* CharacterASC = GetAbilitySystemCharacterComponent())
	{
		CharacterASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UAbilitySystemCharacterComponent* APlayerControllerGameplay::GetAbilitySystemCharacterComponent() const
{
	const APlayerStateGameplay* PlayerStateGameplay = GetPlayerStateGameplay();
	return PlayerStateGameplay ? PlayerStateGameplay->GetAbilitySystemCharacterComponent() : nullptr;
}

APlayerStateGameplay* APlayerControllerGameplay::GetPlayerStateGameplay() const
{
	return CastChecked<APlayerStateGameplay>(PlayerState, ECastCheckedType::NullAllowed);
}
