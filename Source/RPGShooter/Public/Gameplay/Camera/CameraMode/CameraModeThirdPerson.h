// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Camera/CameraMode/CameraModeBase.h"
#include "CameraModeThirdPerson.generated.h"

class UCurveVector;

/**
 * 
 */
UCLASS()
class RPGSHOOTER_API UCameraModeThirdPerson : public UCameraModeBase
{
	GENERATED_BODY()

public:
	UCameraModeThirdPerson();

protected:
	virtual void UpdateView(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Camera|ThirdPerson", Meta = (EditCondition = "!bUseRuntimeFloatCurves"))
		const UCurveVector* TargetOffsetCurve;
	
};
