// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputConfigData.generated.h"

class UInputAction;
class UInputMappingContext;


USTRUCT(BlueprintType)
struct RPGSHOOTER_API FTagInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
		FGameplayTag InputTag;
};


UCLASS(BlueprintType, Const)
class RPGSHOOTER_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindInputActionForTag(const FGameplayTag& InputTag) const;
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputMapping"))
		UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
		TArray<FTagInputAction> TagInputActions;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
		TArray<FTagInputAction> TagAbilityInputActions;
};
