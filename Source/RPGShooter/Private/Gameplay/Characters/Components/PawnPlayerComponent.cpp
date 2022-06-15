// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/Components/PawnPlayerComponent.h"
#include "Player/PlayerStateGameplay.h"
#include "Characters/Components/PawnGameplayComponent.h"
#include "DebugLogger.h"
#include "Player/PlayerControllerGameplay.h"

UPawnPlayerComponent::UPawnPlayerComponent()
{
	bPawnHasInitialized = false;
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
	if (!Pawn) return;

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


	bPawnHasInitialized = true;
}

void UPawnPlayerComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	const APlayerController* PlayerController = GetController<APlayerController>();
	check(PlayerController);

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	DEBUG_POINT();

// 	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
// 	check(InputSubsystem);
// 
// 	InputSubsystem->ClearAllMappings();
// 
// 	const UGameplayPawnComponent* GameplayPawnComponent = UGameplayPawnComponent::FindGameplayPawnComponent(Pawn);
// 	const UPawnData* PawnData = GameplayPawnComponent ? GameplayPawnComponent->GetPawnData<UPawnData>() : nullptr;
// 	const UInputConfigData* InputConfig = PawnData ? PawnData->InputConfig : nullptr;
// 
// 	if (InputConfig)
// 	{
// 		UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(InputComponent);
// 		InputSubsystem->AddMappingContext(InputConfig->InputMappingContext, 1);
// 
// 		PlayerInputComponent->BindTagAction(InputConfig, TAG_Input_Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
// 		PlayerInputComponent->BindTagAction(InputConfig, TAG_Input_Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
// 
// 		TArray<uint32> BindHandles;
// 		PlayerInputComponent->BindTagAbilityActions(InputConfig, this, &ThisClass::InputAbilityPressed, &ThisClass::InputAbilityReleased, BindHandles);
// 	}
// 
// 	if (ensure(!bReadyToBindInputs)) { bReadyToBindInputs = true; }
}
