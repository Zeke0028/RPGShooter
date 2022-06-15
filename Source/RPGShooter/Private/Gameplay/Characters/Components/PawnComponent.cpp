// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Characters/Components/PawnComponent.h"


UPawnComponent::UPawnComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}
