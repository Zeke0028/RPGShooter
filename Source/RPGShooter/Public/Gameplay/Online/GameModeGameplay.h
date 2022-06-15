// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeGameplay.generated.h"

class UPawnData;

/**
 * 
 */
UCLASS()
class RPGSHOOTER_API AGameModeGameplay : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameModeGameplay();

	UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	const UPawnData* GetPawnDataForController(const AController* Controller) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
		UPawnData* DefaultPawnData;
};
