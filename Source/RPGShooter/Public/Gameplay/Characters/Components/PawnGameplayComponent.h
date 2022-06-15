// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/Components/PawnComponent.h"
#include "PawnGameplayComponent.generated.h"

class UAbilitySystemCharacterComponent;

/**
 * 
 */
UCLASS()
class RPGSHOOTER_API UPawnGameplayComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UPawnGameplayComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeAbilitySystem(UAbilitySystemCharacterComponent* InCharacterASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();

	void HandleControllerChanged();
	void HandlePlayerStateReplicated();
	void SetupPlayerInputComponent();

	bool UpdatePawnReady();

protected:
	void OnRegister() override;

public:
	UFUNCTION(BlueprintPure, Category = "Gameplay|Pawn")
		static UPawnGameplayComponent* FindPawnGameplayComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UPawnGameplayComponent>() : nullptr); }

	UFUNCTION(BlueprintPure, Category = "Gameplay|Pawn")
		UAbilitySystemCharacterComponent* GetAbilitySystemCharacterComponent() const { return AbilitySystemComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay|Pawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
		bool IsPawnReadyToInitialize() const { return bPawnReady; }

public:
	void OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	void OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

protected:
	FSimpleMulticastDelegate OnPawnReadyToInitialize;
	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

protected:
	UPROPERTY()
		UAbilitySystemCharacterComponent* AbilitySystemComponent;

	int32 bPawnReady : 1;
};
