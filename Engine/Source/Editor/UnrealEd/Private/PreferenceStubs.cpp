// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#include "UnrealEd.h"

// @todo find a better place for all of this, preferably in the appropriate modules
// though this would require the classes to be relocated as well

//
// UCascadeOptions
// 
UCascadeOptions::UCascadeOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
////////////////// UPhATSimOptions ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
UPhATSimOptions::UPhATSimOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PhysicsBlend = 1.0f;
}

UMaterialEditorOptions::UMaterialEditorOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


UCurveEdOptions::UCurveEdOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPersonaOptions::UPersonaOptions(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, DefaultLocalAxesSelection(2)
{
	ViewModeIndex = VMI_Lit;
}

void UPersonaOptions::SetViewportBackgroundColor( const FLinearColor& InViewportBackgroundColor)
{
	ViewportBackgroundColor = InViewportBackgroundColor;
	SaveConfig();
}

void UPersonaOptions::SetShowGrid( bool bInShowGrid )
{
	bShowGrid = bInShowGrid;
	SaveConfig();
}

void UPersonaOptions::SetHighlightOrigin( bool bInHighlightOrigin )
{
	bHighlightOrigin = bInHighlightOrigin;
	SaveConfig();
}

void UPersonaOptions::SetGridSize( int32 InGridSize )
{
	GridSize = InGridSize;
	SaveConfig();
}

void UPersonaOptions::SetViewModeIndex( int32 InViewModeIndex )
{
	ViewModeIndex = InViewModeIndex;
	SaveConfig();
}

void UPersonaOptions::SetShowFloor( bool bInShowFloor )
{
	bShowFloor = bInShowFloor;
	SaveConfig();
}
void UPersonaOptions::SetShowSky( bool bInShowSky )
{
	bShowSky = bInShowSky;
	SaveConfig();
}

void UPersonaOptions::SetMuteAudio( bool bInMuteAudio )
{
	bMuteAudio = bInMuteAudio;
	SaveConfig();
}

void UPersonaOptions::SetViewFOV( float InViewFOV )
{
	ViewFOV = InViewFOV;
	SaveConfig();
}

void UPersonaOptions::SetDefaultLocalAxesSelection( uint32 InDefaultLocalAxesSelection )
{
	DefaultLocalAxesSelection = InDefaultLocalAxesSelection;
	SaveConfig();
}


void UPersonaOptions::SetShowMeshStats( int32 InShowMeshStats )
{
	ShowMeshStats = InShowMeshStats;
	SaveConfig();
}

