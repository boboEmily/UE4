// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AIModulePrivate.h"
#include "BehaviorTree/BTService.h"

UBTService::UBTService(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
	bNotifyOnSearch = true;
	bTickIntervals = true;

	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService::TickNode(UBehaviorTreeComponent* OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const float NextTickTime = FMath::FRandRange(FMath::Max(0.0f, Interval - RandomDeviation), (Interval + RandomDeviation));
	SetNextTickTime(NodeMemory, NextTickTime);
}

void UBTService::OnSearchStart(struct FBehaviorTreeSearchData& SearchData)
{
	uint8* NodeMemory = GetNodeMemory<uint8>(SearchData);
	TickNode(SearchData.OwnerComp, NodeMemory, 0.0f);
}

void UBTService::NotifyParentActivation(struct FBehaviorTreeSearchData& SearchData)
{
	if (bNotifyOnSearch)
	{
		UBTNode* NodeOb = bCreateNodeInstance ? GetNodeInstance(SearchData) : this;
		if (NodeOb)
		{
			((UBTService*)NodeOb)->OnSearchStart(SearchData);
		}
	}
}

FString UBTService::GetStaticTickIntervalDescription() const
{
	FString IntervalDesc = (RandomDeviation > 0.0f) ?
		FString::Printf(TEXT("%.2fs..%.2fs"), FMath::Max(0.0f, Interval - RandomDeviation), (Interval + RandomDeviation)) :
		FString::Printf(TEXT("%.2fs"), Interval);

	return FString::Printf(TEXT("tick every %s"), *IntervalDesc);
}

FString UBTService::GetStaticServiceDescription() const
{
	return GetStaticTickIntervalDescription();
}

FString UBTService::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), *UBehaviorTreeTypes::GetShortTypeName(this), *GetStaticServiceDescription());
}

#if WITH_EDITOR

FName UBTService::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Service.Icon");
}

#endif // WITH_EDITOR
