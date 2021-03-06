// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "PawnPlayerComponent.generated.h"

struct FInputActionValue;
class UCameraModeBase;

/**
 * 
 */
UCLASS()
class RPGSHOOTER_API UPawnPlayerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UPawnPlayerComponent();

	virtual bool IsPawnComponentReadyToInitialize() const override;

protected:
	virtual void OnRegister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnPawnReadyToInitialize();

	virtual void InitializePlayerInput(UInputComponent* InputComponent);

	void InputMove(const FInputActionValue& InputActionValue);
	void InputLook(const FInputActionValue& InputActionValue);

	void InputAbilityPressed(FGameplayTag InputTag);
	void InputAbilityReleased(FGameplayTag InputTag);

	TSubclassOf<UCameraModeBase> DetermineCameraMode() const;

protected:

	bool bPawnHasInitialized;
	bool bReadyToBindInputs;
};
