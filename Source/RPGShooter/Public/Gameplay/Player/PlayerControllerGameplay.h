// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerGameplay.generated.h"

class UAbilitySystemCharacterComponent;
class APlayerStateGameplay;


UCLASS()
class RPGSHOOTER_API APlayerControllerGameplay : public APlayerController
{
	GENERATED_BODY()

public:
	void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay|PlayerController")
		UAbilitySystemCharacterComponent* GetAbilitySystemCharacterComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay|PlayerController")
		APlayerStateGameplay* GetPlayerStateGameplay() const;
};
