// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityCharacter.generated.h"

class ACharacterBase;

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive,
	OnSpawn
};

UCLASS( Abstract, HideCategories = Input )
class RPGSHOOTER_API UGameplayAbilityCharacter : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbilityCharacter();

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Ability")
		ACharacterBase* GetCharacterBaseFromActorInfo() const;

public:
	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Activation")
		EAbilityActivationPolicy ActivationPolicy;
};
