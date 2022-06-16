// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnData.generated.h"

class UInputConfigData;
class UCameraModeBase;


UCLASS(BlueprintType, Const)
class RPGSHOOTER_API UPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPawnData();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Pawn")
		TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Camera")
		TSubclassOf<UCameraModeBase> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Input")
		UInputConfigData* InputConfig;

};
