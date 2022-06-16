// Fill out your copyright notice in the Description page of Project Settings.


#include "Generic/Data/AbilityConfigData.h"
#include "GameplayAbilitySpec.h"
#include "GAS/AbilitySystemCharacterComponent.h"
#include "GAS/GameplayAbilityCharacter.h"

void FGrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (!Handle.IsValid())return;

	AbilitySpecHandles.Add(Handle);
}

void FGrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (!Handle.IsValid()) return;

	GameplayEffectHandles.Add(Handle);
}

void FGrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FGrantedHandles::TakeFromAbilitySystem(UAbilitySystemCharacterComponent* ASC)
{
	check(ASC);
	if (!ASC->IsOwnerActorAuthoritative()) return;

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (!Handle.IsValid()) continue;
		ASC->ClearAbility(Handle);
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (!Handle.IsValid()) continue;
		ASC->RemoveActiveGameplayEffect(Handle);
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		ASC->GetSpawnedAttributes_Mutable().Remove(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

void UAbilityConfigData::GiveToAbilitySystem(UAbilitySystemCharacterComponent* ASC, FGrantedHandles* OutGrantedHandles, UObject* SourceObject ) const
{
	check(ASC);
	if (!ASC->IsOwnerActorAuthoritative()) return;

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FGameplayAbilityData& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability)) continue;

		UGameplayAbilityCharacter* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbilityCharacter>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FGameplayEffectData& EffectToGrant = GrantedGameplayEffects[EffectIndex];
		if (!IsValid(EffectToGrant.GameplayEffect)) continue;

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, ASC->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}

	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FAttributeSetData& SetToGrant = GrantedAttributes[SetIndex];
		if (!IsValid(SetToGrant.AttributeSet)) continue;

		UAttributeSet* NewSet = NewObject<UAttributeSet>(ASC->GetOwner(), SetToGrant.AttributeSet);
		ASC->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}
}
