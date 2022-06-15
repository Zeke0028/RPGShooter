// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/Components/PawnGameplayComponent.h"
#include "GAS/AbilitySystemCharacterComponent.h"
#include "LogChannels.h"
#include "Net/UnrealNetwork.h"
#include "Data/PawnData.h"

UPawnGameplayComponent::UPawnGameplayComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	AbilitySystemComponent = nullptr;
	bPawnReady = false;
}

void UPawnGameplayComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPawnGameplayComponent, PawnData);
}

void UPawnGameplayComponent::InitializeAbilitySystem(UAbilitySystemCharacterComponent* InCharacterASC, AActor* InOwnerActor)
{
	check(InCharacterASC);
	check(InOwnerActor);

	if (AbilitySystemComponent == InCharacterASC) return;

	if (AbilitySystemComponent) { UninitializeAbilitySystem(); }

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InCharacterASC->GetAvatarActor();

	UE_LOG(LogRPG, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "),
		*GetNameSafe(InCharacterASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAvatar));

	if (ExistingAvatar && ExistingAvatar != Pawn)
	{
		UE_LOG(LogRPG, Log, TEXT("Existing avatar (authority=%s)"), ExistingAvatar->HasAuthority() ? TEXT("true") : TEXT("false"));

		ensure(!ExistingAvatar->HasAuthority());

		if (UPawnGameplayComponent* OtherPawnGameplayComponent = FindPawnGameplayComponent(ExistingAvatar)) { OtherPawnGameplayComponent->UninitializeAbilitySystem(); }
	}

	AbilitySystemComponent = InCharacterASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	OnAbilitySystemInitialized.Broadcast();
}

void UPawnGameplayComponent::UninitializeAbilitySystem()
{
	if (!AbilitySystemComponent) return;

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities(nullptr, nullptr);
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor()) { AbilitySystemComponent->SetAvatarActor(nullptr); }
		else
		{
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void UPawnGameplayComponent::HandleControllerChanged()
{
	if (AbilitySystemComponent && AbilitySystemComponent->GetAvatarActor() == GetPawnChecked<APawn>())
	{
		AbilitySystemComponent->GetOwnerActor() ? AbilitySystemComponent->RefreshAbilityActorInfo() : UninitializeAbilitySystem();
	}

	UpdatePawnReady();
}

void UPawnGameplayComponent::HandlePlayerStateReplicated()
{
	UpdatePawnReady();
}

void UPawnGameplayComponent::SetupPlayerInputComponent()
{
	UpdatePawnReady();
}

bool UPawnGameplayComponent::UpdatePawnReady()
{
	if (bPawnReady) return true;

	APawn* Pawn = GetPawnChecked<APawn>();
	if (!Pawn) return false;

	const bool bHasAuthority = Pawn->HasAuthority();
	const bool bIsLocallyControlled = Pawn->IsLocallyControlled();

	if (bHasAuthority || bIsLocallyControlled)
	{
		if (!GetController<AController>()) return false;
	}

	TArray<UPawnComponent*> PawnComponents;
	Pawn->GetComponents<UPawnComponent>(PawnComponents);
	for (UPawnComponent* PawnComponent : PawnComponents)
	{
		if (!PawnComponent->IsPawnComponentReadyToInitialize()) return false;
	}

	bPawnReady = true;
	OnPawnReadyToInitialize.Broadcast();

	return true;
}

void UPawnGameplayComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("UPawnGameplayComponent on [%s] can only be added to Pawn actors."), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnGameplayComponents;
	Pawn->GetComponents(UPawnGameplayComponent::StaticClass(), PawnGameplayComponents);
	ensureAlwaysMsgf((PawnGameplayComponents.Num() == 1), TEXT("Only one PawnGameplayComponent should exist on [%s]."), *GetNameSafe(GetOwner()));
}

void UPawnGameplayComponent::OnRep_PawnData()
{
	UpdatePawnReady();
}

void UPawnGameplayComponent::SetPawnData(const UPawnData* InPawnData)
{
	check(InPawnData);
	APawn* Pawn = GetPawnChecked<APawn>();

	bPawnReady = false;

	if (Pawn->GetLocalRole() != ROLE_Authority) return;
	if (PawnData) return;

	PawnData = InPawnData;

	Pawn->ForceNetUpdate();

	UpdatePawnReady();
}

void UPawnGameplayComponent::OnPawnReadyToInitialize_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnPawnReadyToInitialize.IsBoundToObject(Delegate.GetUObject()))
	{
		OnPawnReadyToInitialize.Add(Delegate);
	}

	if (bPawnReady)
	{
		Delegate.Execute();
	}
}

void UPawnGameplayComponent::OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UPawnGameplayComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}
