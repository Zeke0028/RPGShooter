// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerCameraComponent.generated.h"

class UCameraModeStack;
class UCameraModeBase;

DECLARE_DELEGATE_RetVal(TSubclassOf<UCameraModeBase>, FCameraModeDelegate);

UCLASS()
class RPGSHOOTER_API UPlayerCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Gameplay|Camera")
		static UPlayerCameraComponent* FindPlayerCameraComponent(const AActor* Actor)
		{
			return (Actor ? Actor->FindComponentByClass<UPlayerCameraComponent>() : nullptr);
		}

public:
	UPlayerCameraComponent();

	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:
	void OnRegister() override;
	void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();


public:
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	FCameraModeDelegate DetermineCameraModeDelegate;

protected:
	UPROPERTY()
		UCameraModeStack* CameraModeStack;

	float FieldOfViewOffset;
};
