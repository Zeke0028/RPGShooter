// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PlayerStateGameplay.generated.h"

class UAbilitySystemCharacterComponent;
class APlayerControllerGameplay;

UCLASS()
class RPGSHOOTER_API APlayerStateGameplay : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	APlayerStateGameplay();

	virtual void PostInitializeComponents() override;
	virtual void ClientInitialize(AController* Controller) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay|PlayerState")
		APlayerControllerGameplay* GetPlayerControllerGameplay() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay|PlayerState")
		UAbilitySystemCharacterComponent* GetAbilitySystemCharacterComponent() const;
		virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Gameplay|PlayerState")
		UAbilitySystemCharacterComponent* AbilitySystemCharacterComponent;

protected:

	static FName const NAME_AbilitySystemCharacterComponent;
};
