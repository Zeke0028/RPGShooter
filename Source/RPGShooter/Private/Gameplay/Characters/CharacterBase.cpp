// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/PawnGameplayComponent.h"
#include "GAS/AbilitySystemCharacterComponent.h"
#include "DebugLogger.h"

FName const ACharacterBase::NAME_PawnGameplayComponent(TEXT("PawnGameplayComponent"));

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;

	UCapsuleComponent* CollisionCapsuleComponent = GetCapsuleComponent();
	check(CollisionCapsuleComponent);
	CollisionCapsuleComponent->InitCapsuleSize(40.0f, 90.0f);

	USkeletalMeshComponent* MeshComponent = GetMesh();
	check(MeshComponent);
	MeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	PawnGameplayComponent = CreateDefaultSubobject<UPawnGameplayComponent>(NAME_PawnGameplayComponent);
	PawnGameplayComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnGameplayComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
}

void ACharacterBase::OnAbilitySystemInitialized()
{
	UAbilitySystemCharacterComponent* CharacterASC = GetAbilitySystemCharacterComponent();
	check(CharacterASC);

	DEBUG_POINT();
}

void ACharacterBase::OnAbilitySystemUninitialized()
{

}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnGameplayComponent->HandleControllerChanged();
}

void ACharacterBase::UnPossessed()
{
	AController* const OldController = Controller;

	Super::UnPossessed();

	PawnGameplayComponent->HandleControllerChanged();
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnGameplayComponent->SetupPlayerInputComponent();
}

void ACharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	PawnGameplayComponent->HandleControllerChanged();
}

void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	PawnGameplayComponent->HandlePlayerStateReplicated();
}

UAbilitySystemCharacterComponent* ACharacterBase::GetAbilitySystemCharacterComponent() const
{
	return PawnGameplayComponent->GetAbilitySystemCharacterComponent();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return GetAbilitySystemCharacterComponent();
}
