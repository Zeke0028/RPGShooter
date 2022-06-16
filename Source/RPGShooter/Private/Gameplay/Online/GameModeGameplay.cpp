// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Online/GameModeGameplay.h"
#include "Gameplay/Player/PlayerControllerGameplay.h"
#include "Gameplay/Player/PlayerStateGameplay.h"
#include "Gameplay/Characters/CharacterPlayer.h"
#include "Data/PawnData.h"
#include "Characters/Components/PawnGameplayComponent.h"


AGameModeGameplay::AGameModeGameplay()
{
	PlayerControllerClass = APlayerControllerGameplay::StaticClass();
	PlayerStateClass = APlayerStateGameplay::StaticClass();
	DefaultPawnClass = ACharacterPlayer::StaticClass();
}

UClass* AGameModeGameplay::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass) return PawnData->PawnClass;
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* AGameModeGameplay::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	if (!PawnClass) return nullptr;

	APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if (!SpawnedPawn) return nullptr;

	UPawnGameplayComponent* PawnGameplayComponent = UPawnGameplayComponent::FindPawnGameplayComponent(SpawnedPawn);
	const UPawnData* PawnData = GetPawnDataForController(NewPlayer);

	if (PawnGameplayComponent && PawnData)
	{
		PawnGameplayComponent->SetPawnData(PawnData);
	}

	SpawnedPawn->FinishSpawning(SpawnTransform);

	return SpawnedPawn;
}

const UPawnData* AGameModeGameplay::GetPawnDataForController(const AController* Controller) const
{
	if (Controller)
	{
		if (const APlayerStateGameplay* PlayerState = Controller->GetPlayerState<APlayerStateGameplay>())
			if (const UPawnData* PawnData = PlayerState->PlayerPawnData) return PawnData;
	}

	if (DefaultPawnData) return DefaultPawnData;

	return nullptr;
}
