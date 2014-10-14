// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemPrivatePCH.h"
#include "GameplayAbilityTargetActor.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayAbility.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------
//
//	AGameplayAbilityTargetActor
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------

AGameplayAbilityTargetActor::AGameplayAbilityTargetActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StaticTargetFunction = false;
	ShouldProduceTargetDataOnServer = false;
	bDebug = false;
	bDestroyOnConfirmation = true;
}

void AGameplayAbilityTargetActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGameplayAbilityTargetActor, StartLocation);
	DOREPLIFETIME(AGameplayAbilityTargetActor, SourceActor);
	DOREPLIFETIME(AGameplayAbilityTargetActor, bDebug);
	DOREPLIFETIME(AGameplayAbilityTargetActor, bDestroyOnConfirmation);
}

void AGameplayAbilityTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
}

bool AGameplayAbilityTargetActor::IsConfirmTargetingAllowed()
{
	return true;
}

void AGameplayAbilityTargetActor::ConfirmTargetingAndContinue()
{
	check(ShouldProduceTargetData());
	if (IsConfirmTargetingAllowed())
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
	else
	{
		NotifyPlayerControllerOfRejectedConfirmation();
	}
}

void AGameplayAbilityTargetActor::ConfirmTargeting()
{
	if (IsConfirmTargetingAllowed())
	{
		ConfirmTargetingAndContinue();
		if (bDestroyOnConfirmation)
		{
			Destroy();
		}
	}
	else
	{
		NotifyPlayerControllerOfRejectedConfirmation();
	}
}

void AGameplayAbilityTargetActor::NotifyPlayerControllerOfRejectedConfirmation()
{
	if (OwningAbility && OwningAbility->IsInstantiated())
	{
		if (APlayerController* PC = OwningAbility->GetActorInfo().PlayerController.Get())
		{
			if (FGameplayAbilitySpec* AbilitySpec = OwningAbility->GetCurrentAbilitySpec())
			{
				PC->ClientNotifyRejectedAbilityConfirmation(AbilitySpec->InputID);
			}
		}
	}
}

/** Outside code is saying 'stop everything and just forget about it' */
void AGameplayAbilityTargetActor::CancelTargeting()
{
	CanceledDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	Destroy();
}

bool AGameplayAbilityTargetActor::IsNetRelevantFor(class APlayerController* RealViewer, AActor* Viewer, const FVector& SrcLocation)
{
	//The player who created the ability doesn't need to be updated about it - there should be local prediction in place.
	if (RealViewer == MasterPC)
	{
		return false;
	}

	return Super::IsNetRelevantFor(RealViewer, Viewer, SrcLocation);
}

bool AGameplayAbilityTargetActor::GetReplicates() const
{
	return bReplicates;
}

FGameplayAbilityTargetDataHandle AGameplayAbilityTargetActor::StaticGetTargetData(UWorld * World, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) const
{
	return FGameplayAbilityTargetDataHandle();
}

bool AGameplayAbilityTargetActor::OnReplicatedTargetDataReceived(FGameplayAbilityTargetDataHandle& Data) const
{
	return true;
}

bool AGameplayAbilityTargetActor::ShouldProduceTargetData() const
{
	// return true if we are locally owned, or (we are the server and this is a gameplaytarget ability that can produce target data server side)
	return (MasterPC && (MasterPC->IsLocalController() || ShouldProduceTargetDataOnServer));
}

void AGameplayAbilityTargetActor::BindToConfirmCancelInputs()
{
	check(OwningAbility);

	UAbilitySystemComponent* ASC = OwningAbility->GetCurrentActorInfo()->AbilitySystemComponent.Get();
	if (ASC)
	{
		ASC->ConfirmCallbacks.AddDynamic(this, &AGameplayAbilityTargetActor::ConfirmTargeting);
		ASC->CancelCallbacks.AddDynamic(this, &AGameplayAbilityTargetActor::CancelTargeting);
	}
}