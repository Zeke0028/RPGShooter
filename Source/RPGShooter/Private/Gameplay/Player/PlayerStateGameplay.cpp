// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Player/PlayerStateGameplay.h"
#include "GAS/AbilitySystemCharacterComponent.h"
#include "Player/PlayerControllerGameplay.h"
#include "Characters/Components/PawnGameplayComponent.h"

FName const APlayerStateGameplay::NAME_AbilitySystemCharacterComponent(TEXT("AbilitySystemCharacterComponent"));

APlayerStateGameplay::APlayerStateGameplay()
{
	NetUpdateFrequency = 100.0f;

	AbilitySystemCharacterComponent = CreateDefaultSubobject<UAbilitySystemCharacterComponent>(NAME_AbilitySystemCharacterComponent);
	AbilitySystemCharacterComponent->SetIsReplicated(true);
	AbilitySystemCharacterComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

void APlayerStateGameplay::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld() || !GetWorld()->IsGameWorld()) return;

	check(AbilitySystemCharacterComponent);
	AbilitySystemCharacterComponent->InitAbilityActorInfo(this, GetPawn());
}

void APlayerStateGameplay::ClientInitialize(AController* Controller)
{
	Super::ClientInitialize(Controller);

	if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(GetPawn()))
	{
		PawnGameplayComponent->UpdatePawnReady();
	}
}

APlayerControllerGameplay* APlayerStateGameplay::GetPlayerControllerGameplay() const
{
	return Cast<APlayerControllerGameplay>(GetOwner());
}

UAbilitySystemCharacterComponent* APlayerStateGameplay::GetAbilitySystemCharacterComponent() const
{
	return AbilitySystemCharacterComponent;
}

UAbilitySystemComponent* APlayerStateGameplay::GetAbilitySystemComponent() const
{
	return GetAbilitySystemCharacterComponent();
}
