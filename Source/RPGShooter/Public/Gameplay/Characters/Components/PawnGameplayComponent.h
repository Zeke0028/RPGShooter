// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/Components/PawnComponent.h"
#include "PawnGameplayComponent.generated.h"

class UAbilitySystemCharacterComponent;
class UPawnData;

/**
 * 
 */
UCLASS()
class RPGSHOOTER_API UPawnGameplayComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Gameplay|Pawn")
		static UPawnGameplayComponent* FindPawnGameplayComponent(const AActor* Actor)
		{
			return (Actor ? Actor->FindComponentByClass<UPawnGameplayComponent>() : nullptr);
		}

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

	UFUNCTION()
		void OnRep_PawnData();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay|Pawn", Meta = (ExpandBoolAsExecs = "ReturnValue"))
		bool IsPawnReadyToInitialize() const { return bPawnReady; }

	void SetPawnData(const UPawnData* InPawnData);

	template <class T>
	const T* GetPawnData() const
	{
		return Cast<T>(PawnData);
	}

	UFUNCTION(BlueprintPure, Category = "Gameplay|Pawn")
		UAbilitySystemCharacterComponent* GetAbilitySystemCharacterComponent() const { return AbilitySystemComponent; }

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

	UPROPERTY(EditInstanceOnly, ReplicatedUsing = OnRep_PawnData, Category = "Gameplay|Pawn")
		const UPawnData* PawnData;

	int32 bPawnReady : 1;
};
