// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemCharacterComponent.generated.h"


UCLASS()
class RPGSHOOTER_API UAbilitySystemCharacterComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UAbilitySystemCharacterComponent();

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:

	void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:

	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
