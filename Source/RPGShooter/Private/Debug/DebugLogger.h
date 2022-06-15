// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDebug, Log, All);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NETMODE                                                                                   \
	(((GEngine == nullptr) || (GetWorld() == nullptr))			  ? FString("")                   \
		: (GEngine->GetNetMode(GetWorld()) == NM_Client)		  ? FString("[Client] ")          \
		: (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)	  ? FString("[ListenServer] ")    \
		: (GEngine->GetNetMode(GetWorld()) == NM_DedicatedServer) ? FString("[DedicatedServer] ") \
																  : FString("[Standalone] "))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CURRENT_CLASS_FUNC (FString(__FUNCTION__))

#define CURRENT_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))))

#define CURRENT_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2))

#define CURRENT_LINE (FString::FromInt(__LINE__))

#define CURRENT_CLASS_LINE (CURRENT_CLASS + "(" + CURRENT_LINE + ")")

#define CURRENT_CLASS_FUNC_LINE (CURRENT_CLASS_FUNC + "(" + CURRENT_LINE + ")")

#define CURRENT_FUNCSIG (FString(__FUNCSIG__))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG_POINT() UE_LOG(LogDebug, Warning, TEXT("%s : %s"), *CURRENT_CLASS_FUNC_LINE, *GetNameSafe(this));

#define DEBUG_POINT_NET() UE_LOG(LogDebug, Warning, TEXT("%s%s : %s"), *NETMODE, *CURRENT_CLASS_FUNC_LINE, *GetNameSafe(this));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG_LOG(Format, ...)                                                                                           \
	UE_LOG(LogDebug, Warning, TEXT("%s : %s"), *CURRENT_CLASS_FUNC_LINE, *FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
	if (GEngine != nullptr) { GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *(CURRENT_CLASS_FUNC_LINE + " : " + FString::Printf(TEXT(Format), ##__VA_ARGS__))); }

#define DEBUG_LOG_NET(Format, ...) UE_LOG(LogDebug, Warning, TEXT("%s%s : %s"), *NETMODE, *CURRENT_CLASS_FUNC_LINE, *FString::Printf(TEXT(Format), ##__VA_ARGS__));
