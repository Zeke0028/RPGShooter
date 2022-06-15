// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Camera/CameraModeStack.h"
#include "Camera/CameraMode/CameraModeBase.h"


UCameraModeStack::UCameraModeStack()
{
	bIsActive = true;
}

void UCameraModeStack::ActivateStack()
{
	if (bIsActive) return;
	bIsActive = true;

	for (UCameraModeBase* CameraMode : CameraModeStack)
	{
		check(CameraMode);
		CameraMode->OnActivation();
	}
}

void UCameraModeStack::DeactivateStack()
{
	if (!bIsActive) return;
	bIsActive = false;

	for (UCameraModeBase* CameraMode : CameraModeStack)
	{
		check(CameraMode);
		CameraMode->OnDeactivation();
	}
}

void UCameraModeStack::PushCameraMode(TSubclassOf<UCameraModeBase> CameraModeClass)
{
	if (!CameraModeClass) return;

	UCameraModeBase* CameraMode = GetCameraModeInstance(CameraModeClass);
	check(CameraMode);

	int32 StackSize = CameraModeStack.Num();
	if (StackSize > 0 && CameraModeStack[0] == CameraMode) return;

	int32 ExistingStackIndex = INDEX_NONE;
	float ExistingStackContribution = 1.0f;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		if (CameraModeStack[StackIndex] == CameraMode)
		{
			ExistingStackIndex = StackIndex;
			ExistingStackContribution *= CameraMode->GetBlendWeight();
			break;
		}
		ExistingStackContribution *= (1.0f - CameraModeStack[StackIndex]->GetBlendWeight());
	}

	if (ExistingStackIndex != INDEX_NONE)
	{
		CameraModeStack.RemoveAt(ExistingStackIndex);
		StackSize--;
	}
	else
	{
		ExistingStackContribution = 0.0f;
	}

	const bool bShouldBlend = (CameraMode->GetBlendTime() > 0.0f && StackSize > 0);
	const float BlendWeight = (bShouldBlend ? ExistingStackContribution : 1.0f);

	CameraMode->SetBlendWeight(BlendWeight);

	CameraModeStack.Insert(CameraMode, 0);
	CameraModeStack.Last()->SetBlendWeight(1.0f);

	if (ExistingStackIndex == INDEX_NONE) { CameraMode->OnActivation(); }
}

bool UCameraModeStack::EvaluateStack(float DeltaTime, FCameraModeView& OutCameraModeView)
{
	if (!bIsActive) return false;

	UpdateStack(DeltaTime);
	BlendStack(OutCameraModeView);

	return true;
}

void UCameraModeStack::GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const
{
	if (CameraModeStack.Num() == 0)
	{
		OutWeightOfTopLayer = 1.0f;
		OutTagOfTopLayer = FGameplayTag();
		return;
	}

	UCameraModeBase* TopEntry = CameraModeStack.Last();
	check(TopEntry);
	OutWeightOfTopLayer = TopEntry->GetBlendWeight();
	OutTagOfTopLayer = TopEntry->GetCameraTypeTag();
}

void UCameraModeStack::UpdateStack(float DeltaTime)
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0) return;

	int32 RemoveCount = 0;
	int32 RemoveIndex = INDEX_NONE;

	for (int32 StackIndex = 0; StackIndex < StackSize; ++StackIndex)
	{
		UCameraModeBase* CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		CameraMode->UpdateCameraMode(DeltaTime);

		if (CameraMode->GetBlendWeight() >= 1.0f)
		{
			RemoveIndex = (StackIndex + 1);
			RemoveCount = (StackSize - RemoveIndex);
			break;
		}
	}

	if (RemoveCount > 0)
	{
		for (int32 StackIndex = RemoveIndex; StackIndex < StackSize; ++StackIndex)
		{
			UCameraModeBase* CameraMode = CameraModeStack[StackIndex];
			check(CameraMode);

			CameraMode->OnDeactivation();
		}

		CameraModeStack.RemoveAt(RemoveIndex, RemoveCount);
	}
}

void UCameraModeStack::BlendStack(FCameraModeView& OutCameraModeView) const
{
	const int32 StackSize = CameraModeStack.Num();
	if (StackSize <= 0) return;

	const UCameraModeBase* CameraMode = CameraModeStack[StackSize - 1];
	check(CameraMode);

	OutCameraModeView = CameraMode->GetCameraModeView();

	for (int32 StackIndex = (StackSize - 2); StackIndex >= 0; --StackIndex)
	{
		CameraMode = CameraModeStack[StackIndex];
		check(CameraMode);

		OutCameraModeView.Blend(CameraMode->GetCameraModeView(), CameraMode->GetBlendWeight());
	}
}

UCameraModeBase* UCameraModeStack::GetCameraModeInstance(TSubclassOf<UCameraModeBase> CameraModeClass)
{
	check(CameraModeClass);

	for (UCameraModeBase* CameraMode : CameraModeInstances)
	{
		if (CameraMode && CameraMode->GetClass() == CameraModeClass) return CameraMode;
	}

	UCameraModeBase* CameraMode = NewObject<UCameraModeBase>(GetOuter(), CameraModeClass, NAME_None, RF_NoFlags);
	check(CameraMode);
	CameraModeInstances.Add(CameraMode);

	return CameraMode;
}
