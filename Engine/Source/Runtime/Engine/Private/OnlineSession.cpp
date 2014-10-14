// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

/*=============================================================================
	GameSession.cpp: GameSession code.
=============================================================================*/

#include "EnginePrivate.h"


UOnlineSession::UOnlineSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOnlineSession::HandleDisconnect(UWorld *World, UNetDriver *NetDriver)
{
	// Let the engine cleanup this disconnect
	GEngine->HandleDisconnect(World, NetDriver);
}
