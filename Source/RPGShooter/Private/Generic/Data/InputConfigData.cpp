// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/Data/InputConfigData.h"

const UInputAction* UInputConfigData::FindInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTagInputAction& Action : TagInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag)) return Action.InputAction;
	}

	return nullptr;
}

const UInputAction* UInputConfigData::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FTagInputAction& Action : TagAbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
