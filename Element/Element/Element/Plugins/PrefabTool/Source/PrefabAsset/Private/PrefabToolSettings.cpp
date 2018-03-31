// Copyright 2017 marynate. All Rights Reserved.

#include "PrefabToolSettings.h"

//////////////////////////////////////////////////////////////////////////
// UPrefabToolEditorSettings

UPrefabToolSettings::UPrefabToolSettings()
	: bReplaceActorsWithCreatedPrefab(false)
	, bNestedPrefabSupport(true)
	, bUpdateAllPrefabActorsWhenOpenMap(true)
	, bCheckPrefabChangeBeforeUpdateAllPrefabActorsWhenOpenMap(true)
	, bUpdateAllPrefabActorsWhenApply(true)
	, bEnablePrefabComponentVisualizer(true)

	// Experimental
	, bLockPrefabSelectionByDefault(true)
	, bDisableLockPrefabSelectionFeature(false)
	, bSupportGenerateBlueprint(false)
	, bForceApplyPerInstanceVertexColor(false)
	, bBSPBrushSupport(false)

	//, bCreateGroupWhenAddPrefabToLevel(false)
	, bEnablePrefabTextEditor(false)
	, bDebugMode(false)
{
	if (!IsRunningCommandlet())
	{
		//ConnectedPrefabIconTextureName = FSoftObjectPath("/PrefabTool/Icons/S_PrefabConnected.S_PrefabConnected");
	}
}
