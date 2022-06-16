// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/GameplayAbilityCharacter.h"
#include "AbilityJump.generated.h"


UCLASS(Abstract)
class RPGSHOOTER_API UAbilityJump : public UGameplayAbilityCharacter
{
	GENERATED_BODY()

public:
	UAbilityJump();

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Ability")
		void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Ability")
		void CharacterJumpStop();
};
