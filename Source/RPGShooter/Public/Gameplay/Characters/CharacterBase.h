// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

class UAbilitySystemCharacterComponent;
class UAbilitySystemComponent;
class UPawnGameplayComponent;

UCLASS()
class RPGSHOOTER_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay|Character")
		virtual UAbilitySystemCharacterComponent* GetAbilitySystemCharacterComponent() const;
		virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay|Character", Meta = (AllowPrivateAccess = "true"))
		UPawnGameplayComponent* PawnGameplayComponent;

protected:
	static FName const NAME_PawnGameplayComponent;

};
