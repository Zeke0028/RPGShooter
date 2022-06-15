// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Camera/PlayerCameraComponent.h"
#include "Camera/CameraModeStack.h"
#include "Camera/CameraMode/CameraModeBase.h"

UPlayerCameraComponent::UPlayerCameraComponent()
{
	CameraModeStack = nullptr;
	FieldOfViewOffset = 0.0f;
}

void UPlayerCameraComponent::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	check(CameraModeStack);
	CameraModeStack->GetBlendInfo(OutWeightOfTopLayer, OutTagOfTopLayer);
}

void UPlayerCameraComponent::OnRegister()
{
	Super::OnRegister();
	if (CameraModeStack) return;

	CameraModeStack = NewObject<UCameraModeStack>(this);
	check(CameraModeStack);
}

void UPlayerCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();

	FCameraModeView CameraModeView;
	CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);

	if (APawn* TargetPawn = Cast<APawn>(GetTargetActor()))
	{
		if (APlayerController* PC = TargetPawn->GetController<APlayerController>())
		{
			PC->SetControlRotation(CameraModeView.ControlRotation);
		}
	}

	CameraModeView.FieldOfView += FieldOfViewOffset;
	FieldOfViewOffset = 0.0f;

	SetWorldLocationAndRotation(CameraModeView.Location, CameraModeView.Rotation);
	FieldOfView = CameraModeView.FieldOfView;

	DesiredView.Location = CameraModeView.Location;
	DesiredView.Rotation = CameraModeView.Rotation;
	DesiredView.FOV = CameraModeView.FieldOfView;
	DesiredView.OrthoWidth = OrthoWidth;
	DesiredView.OrthoNearClipPlane = OrthoNearClipPlane;
	DesiredView.OrthoFarClipPlane = OrthoFarClipPlane;
	DesiredView.AspectRatio = AspectRatio;
	DesiredView.bConstrainAspectRatio = bConstrainAspectRatio;
	DesiredView.bUseFieldOfViewForLOD = bUseFieldOfViewForLOD;
	DesiredView.ProjectionMode = ProjectionMode;

	DesiredView.PostProcessBlendWeight = PostProcessBlendWeight;
	if (PostProcessBlendWeight > 0.0f)
	{
		DesiredView.PostProcessSettings = PostProcessSettings;
	}
}

void UPlayerCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (!CameraModeStack->IsStackActivate()) return;
	if (!DetermineCameraModeDelegate.IsBound()) return;

	if (const TSubclassOf<UCameraModeBase> CameraMode = DetermineCameraModeDelegate.Execute())
	{
		CameraModeStack->PushCameraMode(CameraMode);
	}
}
