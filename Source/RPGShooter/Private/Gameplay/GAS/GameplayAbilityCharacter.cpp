// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/GameplayAbilityCharacter.h"
#include "Characters/CharacterBase.h"

UGameplayAbilityCharacter::UGameplayAbilityCharacter()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;

	ActivationPolicy = EAbilityActivationPolicy::OnInputTriggered;
}

ACharacterBase* UGameplayAbilityCharacter::GetCharacterBaseFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ACharacterBase>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}
