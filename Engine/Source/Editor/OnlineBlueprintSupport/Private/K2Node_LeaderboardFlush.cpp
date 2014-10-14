// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "OnlineBlueprintSupportPrivatePCH.h"
#include "LeaderboardFlushCallbackProxy.h"
#include "K2Node_LeaderboardFlush.h"

#define LOCTEXT_NAMESPACE "K2Node"

UK2Node_LeaderboardFlush::UK2Node_LeaderboardFlush(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(ULeaderboardFlushCallbackProxy, CreateProxyObjectForFlush);
	ProxyFactoryClass = ULeaderboardFlushCallbackProxy::StaticClass();

	ProxyClass = ULeaderboardFlushCallbackProxy::StaticClass();
}

FText UK2Node_LeaderboardFlush::GetTooltipText() const
{
	return LOCTEXT("K2Node_LeaderboardFlush_Tooltip", "Flushes leaderboards for a session");
}

FText UK2Node_LeaderboardFlush::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("FlushLeaderboards", "Flush Leaderboards");
}

FText UK2Node_LeaderboardFlush::GetMenuCategory() const
{
	return LOCTEXT("LeaderboardFlushCategory", "Online|Leaderboard");
}

#undef LOCTEXT_NAMESPACE
