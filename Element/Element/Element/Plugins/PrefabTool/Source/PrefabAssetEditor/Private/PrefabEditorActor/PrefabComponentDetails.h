// Copyright 2017 marynate. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"
#include "DetailLayoutBuilder.h"

/**
* Wraps a details panel customized for viewing prefab component
*/
class FPrefabComponentDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	~FPrefabComponentDetails();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

private:
	
	FReply OnSelectPrefabClicked();
	FReply OnRevertPrefabClicked();
	FReply OnApplyPrefabClicked();

	FReply OnConvertPrefabToNormalActorClicked();
	FReply OnDestroyPrefabActorClicked();
	FReply OnDestroyPrefabActorHierarchyClicked();

	FReply OnGenerateBlueprintClicked();
	FReply OnUpdateBlueprintClicked();

	void DoDestroySelectedPrefabActor(bool bDeleteInstanceActors);

	bool HasConnectedPrefab() const;

//	const FTextBlockStyle& GetConnectedTextStyle() const;

	TArray<TWeakObjectPtr<class APrefabActor>> SelectedPrefabActors;
};