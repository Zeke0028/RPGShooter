// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/Components/PawnPlayerComponent.h"
#include "Debug/LogChannels.h"
#include "Player/PlayerStateGameplay.h"
#include "Characters/Components/PawnGameplayComponent.h"
#include "DebugLogger.h"
#include "Player/PlayerControllerGameplay.h"
#include "EnhancedInputSubsystems.h"
#include "Data/InputConfigData.h"
#include "Data/PawnData.h"
#include "NativeGameplayTags.h"
#include "Input/PlayerInputComponent.h"
#include "Camera/PlayerCameraComponent.h"
#include "Camera/CameraMode/CameraModeThirdPerson.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Move, "InputTag.Move");
UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Look, "InputTag.Look");

UPawnPlayerComponent::UPawnPlayerComponent()
{
	bPawnHasInitialized = false;
	bReadyToBindInputs = false;

	DefaultCameraMode = UCameraModeThirdPerson::StaticClass();
}

bool UPawnPlayerComponent::IsPawnComponentReadyToInitialize() const
{
	if (!GetPlayerState<APlayerStateGameplay>()) return false;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return false;

	if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
	{
		const AController* Controller = GetController<AController>();
		const bool bHasControlled = Controller && Controller->PlayerState && Controller->PlayerState->GetOwner() == Controller;

		if (!bHasControlled) return false;
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
	const bool bIsBot = Pawn->IsBotControlled();

	if (bIsLocallyControlled && !bIsBot)
	{
		if (!Pawn->InputComponent) return false;
	}

	return true;
}

void UPawnPlayerComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		UE_LOG(LogRPG, Error,
			TEXT("[UPlayerCharacterComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To "
				"use this component, it MUST be placed on a Pawn Blueprint."));
		return;
	}

	if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn))
	{
		PawnGameplayComponent->OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnPawnReadyToInitialize));
	}
}

void UPawnPlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn))
		{
			PawnGameplayComponent->UninitializeAbilitySystem();
		}
	}

	Super::EndPlay(EndPlayReason);
}

void UPawnPlayerComponent::OnPawnReadyToInitialize()
{
	if (!ensure(!bPawnHasInitialized)) return;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	APlayerStateGameplay* PlayerState = GetPlayerState<APlayerStateGameplay>();
	check(PlayerState);

	if (UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn))
	{
		PawnGameplayComponent->InitializeAbilitySystem(PlayerState->GetAbilitySystemCharacterComponent(), PlayerState);
	}

	if (APlayerControllerGameplay* GameplayPC = GetController<APlayerControllerGameplay>())
	{
		if (Pawn->InputComponent)
		{
			InitializePlayerInput(Pawn->InputComponent);
		}
	}

	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		if (UPlayerCameraComponent* CameraComponent = UPlayerCameraComponent::FindPlayerCameraComponent(Pawn))
		{
			CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
		}
	}

	bPawnHasInitialized = true;
}

void UPawnPlayerComponent::InitializePlayerInput(UInputComponent* InputComponent)
{
	check(InputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(InputSubsystem);
	InputSubsystem->ClearAllMappings();

	const UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(Pawn);
	const UPawnData* PawnData = PawnGameplayComponent ? PawnGameplayComponent->GetPawnData<UPawnData>() : nullptr;
	const UInputConfigData* InputConfig = PawnData ? PawnData->InputConfig : nullptr;

	if (InputConfig)
	{
		UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(InputComponent);
		InputSubsystem->AddMappingContext(InputConfig->InputMappingContext, 1);

		PlayerInputComponent->BindTagAction(InputConfig, TAG_Input_Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		PlayerInputComponent->BindTagAction(InputConfig, TAG_Input_Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
	}

	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
}

void UPawnPlayerComponent::InputMove(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	if (!Controller) return;

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		Pawn->AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		Pawn->AddMovementInput(MovementDirection, Value.Y);
	}
}

void UPawnPlayerComponent::InputLook(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

TSubclassOf<UCameraModeBase> UPawnPlayerComponent::DetermineCameraMode() const
{
	return DefaultCameraMode;
}
