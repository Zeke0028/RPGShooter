// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Camera/CameraMode/CameraModeThirdPerson.h"
#include "Curves/CurveVector.h"

UCameraModeThirdPerson::UCameraModeThirdPerson()
{
	TargetOffsetCurve = nullptr;
}

void UCameraModeThirdPerson::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
