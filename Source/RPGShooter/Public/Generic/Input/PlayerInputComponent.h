// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerInputComponent.generated.h"

class UInputConfigData;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class RPGSHOOTER_API UPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename FuncType>
	void BindTagAction(const UInputConfigData* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindTagAbilityActions(const UInputConfigData* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UPlayerInputComponent::BindTagAction(const UInputConfigData* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	check(InputConfig);
	if (const UInputAction* InputAction = InputConfig->FindInputActionForTag(InputTag))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UPlayerInputComponent::BindTagAbilityActions(const UInputConfigData* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FTagInputAction& InputAction : InputConfig->TagAbilityInputActions)
	{
		if (!InputAction.InputAction || !InputAction.InputTag.IsValid()) continue;

		if (PressedFunc)
		{
			BindHandles.Add(BindAction(InputAction.InputAction, ETriggerEvent::Started, Object, PressedFunc, InputAction.InputTag).GetHandle());
		}

		if (ReleasedFunc)
		{
			BindHandles.Add(BindAction(InputAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, InputAction.InputTag).GetHandle());
		}
	}
}