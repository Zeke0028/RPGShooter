// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Player/PlayerStateGameplay.h"
#include "GAS/AbilitySystemCharacterComponent.h"
#include "Player/PlayerControllerGameplay.h"
#include "Characters/Components/PawnGameplayComponent.h"
#include "Net/UnrealNetwork.h"
#include "Data/PawnData.h"
#include "Net/Core/PushModel/PushModel.h"

FName const APlayerStateGameplay::NAME_AbilitySystemCharacterComponent(TEXT("AbilitySystemCharacterComponent"));

APlayerStateGameplay::APlayerStateGameplay()
{
	NetUpdateFrequency = 100.0f;

	AbilitySystemCharacterComponent = CreateDefaultSubobject<UAbilitySystemCharacterComponent>(NAME_AbilitySystemCharacterComponent);
	AbilitySystemCharacterComponent->SetIsReplicated(true);
	AbilitySystemCharacterComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

void APlayerStateGameplay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
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

void APlayerStateGameplay::SetPawnData(const UPawnData* InPawnData)
{
	check(InPawnData);
	if (GetLocalRole() != ROLE_Authority) return;
	if (PawnData) return;

	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	ForceNetUpdate();
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
