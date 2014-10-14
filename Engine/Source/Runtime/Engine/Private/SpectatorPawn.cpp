// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "EnginePrivate.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/SpectatorPawnMovement.h"

ASpectatorPawn::ASpectatorPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
	.SetDefaultSubobjectClass<USpectatorPawnMovement>(Super::MovementComponentName)
	.DoNotCreateDefaultSubobject(Super::MeshComponentName)
	)
{
	bCanBeDamaged = false;
	SetRemoteRoleForBackwardsCompat(ROLE_None);
	bReplicates = false;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;

	static FName CollisionProfileName(TEXT("Spectator"));
	CollisionComponent->SetCollisionProfileName(CollisionProfileName);
}


void ASpectatorPawn::PossessedBy(class AController* NewController)
{
	AController* const OldController = Controller;
	Controller = NewController;

	// dispatch Blueprint event if necessary
	if (OldController != NewController)
	{
		ReceivePossessed(Controller);
	}
}

