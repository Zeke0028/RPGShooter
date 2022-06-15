// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CameraModeBase.generated.h"

class UPlayerCameraComponent;

UENUM(BlueprintType)
enum class ECameraModeBlendFunction : uint8
{
	Linear,
	EaseIn,
	EaseOut,
	EaseInOut,

	COUNT UMETA(Hidden)
};

struct FCameraModeView
{
public:
	FCameraModeView();

	void Blend(const FCameraModeView& Other, float OtherWeight);

public:

	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};


UCLASS(Abstract, NotBlueprintable)
class RPGSHOOTER_API UCameraModeBase : public UObject
{
	GENERATED_BODY()
	
public:
	UCameraModeBase();

	virtual void OnActivation() {};
	virtual void OnDeactivation() {};

	void UpdateCameraMode(float DeltaTime);

public:
	void SetBlendWeight(float Weight);

	virtual UWorld* GetWorld() const override;
	UPlayerCameraComponent* GetPlayerCameraComponent() const;
	AActor* GetTargetActor() const;

	const FCameraModeView& GetCameraModeView() const { return View; }
	float GetBlendTime() const { return BlendTime; }
	float GetBlendWeight() const { return BlendWeight; }

	FGameplayTag GetCameraTypeTag() const { return CameraTypeTag; }

protected:
	virtual void UpdateView(float DeltaTime);
	virtual void UpdateBlending(float DeltaTime);

	virtual FVector GetPivotLocation() const;
	virtual FRotator GetPivotRotation() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		FGameplayTag CameraTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (Units = deg, UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
		float FieldOfView;
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (Units = deg, UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
		float ViewPitchMin;
	UPROPERTY(EditDefaultsOnly, Category = "View", Meta = (Units = deg, UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
		float ViewPitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Blending", Meta = (Units = s))
		float BlendTime;

	UPROPERTY(EditDefaultsOnly, Category = "Blending")
		ECameraModeBlendFunction BlendFunction;
	UPROPERTY(EditDefaultsOnly, Category = "Blending")
		float BlendExponent;

protected:
	UPROPERTY(transient)
		uint32 bResetInterpolation : 1;

	FCameraModeView View;

	float BlendAlpha;
	float BlendWeight;
};
