// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Characters/Components/PawnComponent.h"
#include "PawnPlayerComponent.generated.h"

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

protected:
	bool bPawnHasInitialized;
};
