// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/CharacterBase.h"
#include "CharacterPlayer.generated.h"

class APlayerControllerGameplay;
class APlayerStateGameplay;
class UPawnPlayerComponent;

UCLASS()
class RPGSHOOTER_API ACharacterPlayer : public ACharacterBase
{
	GENERATED_BODY()

public:
	ACharacterPlayer();

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Character")
		APlayerControllerGameplay* GetPlayerControllerGameplay() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay|Character")
		APlayerStateGameplay* GetPlayerStateGameplay() const;

private:
	UPROPERTY(VisibleAnywhere, Transient, Category = "Gameplay|Character")
		UPawnPlayerComponent* PawnPlayerComponent;

protected:
	static FName const NAME_PawnPlayerComponent;

};
