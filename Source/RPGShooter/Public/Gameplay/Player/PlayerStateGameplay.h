// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PlayerStateGameplay.generated.h"

class UAbilitySystemCharacterComponent;
class APlayerControllerGameplay;
class UPawnData;

UCLASS()
class RPGSHOOTER_API APlayerStateGameplay : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	APlayerStateGameplay();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;
	virtual void ClientInitialize(AController* Controller) override;

	virtual void SetPawnData(const UPawnData* InPawnData);

public:
	template <class T>
	const T* GetPawnData() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay|PlayerState")
		APlayerControllerGameplay* GetPlayerControllerGameplay() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay|PlayerState")
		UAbilitySystemCharacterComponent* GetAbilitySystemCharacterComponent() const;
		virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, Category = Gameplay)
		const UPawnData* PawnData;

private:
	UPROPERTY(VisibleAnywhere, Category = "Gameplay|PlayerState")
		UAbilitySystemCharacterComponent* AbilitySystemCharacterComponent;

protected:

	static FName const NAME_AbilitySystemCharacterComponent;
};

template <class T>
const T* APlayerStateGameplay::GetPawnData() const
{
	return Cast<T>(PawnData);
}