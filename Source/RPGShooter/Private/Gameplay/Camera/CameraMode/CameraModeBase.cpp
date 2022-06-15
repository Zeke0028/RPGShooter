// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Camera/CameraMode/CameraModeBase.h"
#include "Camera/PlayerCameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"


FCameraModeView::FCameraModeView() :
	Location(ForceInit)
	, Rotation(ForceInit)
	, ControlRotation(ForceInit)
	, FieldOfView(80.0f)
{
}

void FCameraModeView::Blend(const FCameraModeView& Other, float OtherWeight)
{
	if (OtherWeight <= 0.0f || OtherWeight >= 1.0f)
	{
		if (OtherWeight >= 1.0f) *this = Other;
		return;
	}

	Location = FMath::Lerp(Location, Other.Location, OtherWeight);

	const FRotator DeltaRotation = (Other.Rotation - Rotation).GetNormalized();
	Rotation = Rotation + (OtherWeight * DeltaRotation);

	const FRotator DeltaControlRotation = (Other.ControlRotation - ControlRotation).GetNormalized();
	ControlRotation = ControlRotation + (OtherWeight * DeltaControlRotation);

	FieldOfView = FMath::Lerp(FieldOfView, Other.FieldOfView, OtherWeight);
}

UCameraModeBase::UCameraModeBase()
{
	FieldOfView = 80.0f;
	ViewPitchMin = -89.9f;
	ViewPitchMax = 89.9f;

	BlendTime = 0.5f;
	BlendFunction = ECameraModeBlendFunction::EaseOut;
	BlendExponent = 4.0f;
	BlendAlpha = 1.0f;
	BlendWeight = 1.0f;
}

void UCameraModeBase::UpdateCameraMode(float DeltaTime)
{
	UpdateView(DeltaTime);
	UpdateBlending(DeltaTime);
}

void UCameraModeBase::SetBlendWeight(float Weight)
{
	BlendWeight = FMath::Clamp(Weight, 0.0f, 1.0f);

	const float InvExponent = (BlendExponent > 0.0f) ? (1.0f / BlendExponent) : 1.0f;

	switch (BlendFunction)
	{
	case ECameraModeBlendFunction::Linear:
		BlendAlpha = BlendWeight;
		break;

	case ECameraModeBlendFunction::EaseIn:
		BlendAlpha = FMath::InterpEaseIn(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case ECameraModeBlendFunction::EaseOut:
		BlendAlpha = FMath::InterpEaseOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	case ECameraModeBlendFunction::EaseInOut:
		BlendAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, BlendWeight, InvExponent);
		break;

	default:
		checkf(false, TEXT("SetBlendWeight: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

UWorld* UCameraModeBase::GetWorld() const
{
	return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
}

UPlayerCameraComponent* UCameraModeBase::GetPlayerCameraComponent() const
{
	return CastChecked<UPlayerCameraComponent>(GetOuter());
}

AActor* UCameraModeBase::GetTargetActor() const
{
	const UPlayerCameraComponent* CameraComponent = GetPlayerCameraComponent();

	return CameraComponent->GetTargetActor();
}

void UCameraModeBase::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
}

void UCameraModeBase::UpdateBlending(float DeltaTime)
{
	if (BlendTime > 0.0f)
	{
		BlendAlpha += (DeltaTime / BlendTime);
		BlendAlpha = FMath::Min(BlendAlpha, 1.0f);
	}
	else
	{
		BlendAlpha = 1.0f;
	}

	const float Exponent = (BlendExponent > 0.0f) ? BlendExponent : 1.0f;

	switch (BlendFunction)
	{
	case ECameraModeBlendFunction::Linear:
		BlendWeight = BlendAlpha;
		break;

	case ECameraModeBlendFunction::EaseIn:
		BlendWeight = FMath::InterpEaseIn(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case ECameraModeBlendFunction::EaseOut:
		BlendWeight = FMath::InterpEaseOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	case ECameraModeBlendFunction::EaseInOut:
		BlendWeight = FMath::InterpEaseInOut(0.0f, 1.0f, BlendAlpha, Exponent);
		break;

	default:
		checkf(false, TEXT("UpdateBlending: Invalid BlendFunction [%d]\n"), (uint8)BlendFunction);
		break;
	}
}

FVector UCameraModeBase::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	const APawn* TargetPawn = Cast<APawn>(TargetActor);
	if (!TargetPawn) return TargetActor->GetActorLocation();

	const ACharacter* TargetCharacter = Cast<ACharacter>(TargetPawn);
	if (!TargetCharacter) return TargetPawn->GetPawnViewLocation();

	const ACharacter* TargetCharacterCDO = TargetCharacter->GetClass()->GetDefaultObject<ACharacter>();
	check(TargetCharacterCDO);

	const UCapsuleComponent* CapsuleComponent = TargetCharacter->GetCapsuleComponent();
	check(CapsuleComponent);

	const UCapsuleComponent* CapsuleComponentCDO = TargetCharacterCDO->GetCapsuleComponent();
	check(CapsuleComponentCDO);

	const float DefaultHalfHeight = CapsuleComponentCDO->GetUnscaledCapsuleHalfHeight();
	const float ActualHalfHeight = CapsuleComponent->GetUnscaledCapsuleHalfHeight();
	const float HeightAdjustment = (DefaultHalfHeight - ActualHalfHeight) + TargetCharacterCDO->BaseEyeHeight;

	return TargetCharacter->GetActorLocation() + (FVector::UpVector * HeightAdjustment);
}

FRotator UCameraModeBase::GetPivotRotation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	if (const APawn* TargetPawn = Cast<APawn>(TargetActor)) { return TargetPawn->GetViewRotation(); }

	return TargetActor->GetActorRotation();
}
