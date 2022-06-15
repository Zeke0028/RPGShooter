// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CameraModeStack.generated.h"

class UCameraModeBase;
struct FCameraModeView;

UCLASS()
class RPGSHOOTER_API UCameraModeStack : public UObject
{
	GENERATED_BODY()
	
public:
	UCameraModeStack();

	void ActivateStack();
	void DeactivateStack();

	void PushCameraMode(TSubclassOf<UCameraModeBase> CameraModeClass);
	bool EvaluateStack(float DeltaTime, FCameraModeView& OutCameraModeView);

	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

	bool IsStackActivate() const { return bIsActive; }

protected:
	void UpdateStack(float DeltaTime);
	void BlendStack(FCameraModeView& OutCameraModeView) const;

	UCameraModeBase* GetCameraModeInstance(TSubclassOf<UCameraModeBase> CameraModeClass);

protected:

	UPROPERTY()
		TArray<UCameraModeBase*> CameraModeInstances;
	UPROPERTY()
		TArray<UCameraModeBase*> CameraModeStack;

	bool bIsActive;

};
