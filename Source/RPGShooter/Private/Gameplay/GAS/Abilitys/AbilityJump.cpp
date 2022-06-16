// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GAS/Abilitys/AbilityJump.h"
#include "Characters/CharacterBase.h"

UAbilityJump::UAbilityJump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UAbilityJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid()) return false;

	const ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	if (!Character || !Character->CanJump()) return false;

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UAbilityJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAbilityJump::CharacterJumpStart()
{
	if (ACharacterBase* Character = GetCharacterBaseFromActorInfo())
	{
		if (!Character->IsLocallyControlled() || Character->bPressedJump) return;

		Character->UnCrouch();
		Character->Jump();
	}
}

void UAbilityJump::CharacterJumpStop()
{
	if (ACharacterBase* Character = GetCharacterBaseFromActorInfo())
	{
		if (!Character->IsLocallyControlled() || !Character->bPressedJump) return;

		Character->StopJumping();
	}
}
