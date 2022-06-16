// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GameplayAbilitySpec.h"
#include "AbilityConfigData.generated.h"

class UGameplayAbilityCharacter;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemCharacterComponent;

USTRUCT(BlueprintType)
struct FGameplayAbilityData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameplayAbilityCharacter> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
		int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
		FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FGameplayEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
		float EffectLevel = 1.0f;
};

USTRUCT(BlueprintType)
struct FAttributeSetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UAttributeSet> AttributeSet;
};

USTRUCT(BlueprintType)
struct FGrantedHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UAbilitySystemCharacterComponent* ASC);

protected:
	UPROPERTY()
		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
		TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
		TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

UCLASS(BlueprintType, Const)
class RPGSHOOTER_API UAbilityConfigData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	void GiveToAbilitySystem(UAbilitySystemCharacterComponent* ASC, FGrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay/Abilities", meta = (TitleProperty = Ability))
		TArray<FGameplayAbilityData> GrantedGameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay/Effects", meta = (TitleProperty = GameplayEffect))
		TArray<FGameplayEffectData> GrantedGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay/Attribute", meta = (TitleProperty = AttributeSet))
		TArray<FAttributeSetData> GrantedAttributes;
};
