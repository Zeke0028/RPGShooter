// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/Components/PawnPlayerComponent.h"
#include "Player/PlayerStateGameplay.h"
#include "Characters/Components/PawnGameplayComponent.h"

UPawnPlayerComponent::UPawnPlayerComponent()
{
	bPawnHasInitialized = false;
}

bool UPawnPlayerComponent::IsPawnComponentReadyToInitialize() const
{
	if (!GetPlayerState<APlayerStateGameplay>()) return false;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return false;

	if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
	{
		const AController* Controller = GetController<AController>();
		const bool bHasControlled = Controller && Controller->PlayerState && Controller->PlayerState->GetOwner() == Controller;

		if (!bHasControlled) return false;
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
	const bool bIsBot = Pawn->IsBotControlled();

	if (bIsLocallyControlled && !bIsBot)
	{
		if (!Pawn->InputComponent) return false;
	}

	return true;
}

void UPawnPlayerComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn))
	{
		PawnGameplayComponent->OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnPawnReadyToInitialize));
	}
}

void UPawnPlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn))
		{
			PawnGameplayComponent->UninitializeAbilitySystem();
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UPawnPlayerComponent::OnPawnReadyToInitialize()
{
	if (!ensure(!bPawnHasInitialized)) return;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	APlayerStateGameplay* PlayerState = GetPlayerState<APlayerStateGameplay>();
	check(PlayerState);

	if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn))
	{
		PawnGameplayComponent->InitializeAbilitySystem(PlayerState->GetAbilitySystemCharacterComponent(), PlayerState);
	}

	bPawnHasInitialized = true;
}
