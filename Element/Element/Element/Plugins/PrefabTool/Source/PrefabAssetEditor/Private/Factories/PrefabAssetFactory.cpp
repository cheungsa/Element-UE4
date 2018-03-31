// Copyright 2017 marynate. All Rights Reserved.

#include "PrefabAssetFactory.h"
#include "PrefabActor.h"
#include "PrefabAsset.h"
#include "PrefabToolHelpers.h"
#include "PrefabToolSettings.h"

#include "Editor.h"
#include "Engine/Selection.h"
#include "Layers/ILayers.h"

UPrefabAssetFactory::UPrefabAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	SupportedClass = UPrefabAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = false;
	bEditorImport = false;
}

UObject* UPrefabAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	const UPrefabToolSettings* PrefabToolSettings = GetDefault<UPrefabToolSettings>();
	const bool bReplaceActorsWithCreatedPrefab = PrefabToolSettings->ShouldReplaceActorsWithCreatedPrefab();

	UPrefabAsset* NewPrefab = NewObject<UPrefabAsset>(InParent, InClass, InName, Flags);
	FTransform ReplaceActorTransform = FTransform::Identity;
	AActor* CommonAttachParentActor = NULL;
	FName CommonAttachSocket = NAME_None;

	TArray<AActor*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

	if (SelectedActors.Num() > 0)
	{
		AActor* LastSelectedActor = SelectedActors.Last();
		check(LastSelectedActor != NULL);
		
		PREFABTOOL_LOG(Display, TEXT("Creating prefab asset from %d selected actors."), SelectedActors.Num());

		if (bReplaceActorsWithCreatedPrefab)
		{
			CommonAttachParentActor = LastSelectedActor->GetAttachParentActor();
			CommonAttachSocket = LastSelectedActor->GetAttachParentSocketName();
		}

		FTransform LastSelectedTransform = LastSelectedActor->GetTransform();
		FVector LastSelectedPivot = LastSelectedTransform.TransformPosition(LastSelectedActor->GetPivotOffset());

		FString CopyData;
		FPrefabToolEditorUtil::edactCopySelectedForNewPrefab(LastSelectedActor->GetWorld(), &CopyData, NewPrefab->GetPrefabTagPrefix(), LastSelectedPivot);
		// UE_LOG(LogTemp, Display, TEXT("Prefab: %s"), *CopyData);

		NewPrefab->SetPrefabContent(FText::FromString(CopyData));

		NewPrefab->SetPrefabPivot(FVector::ZeroVector);

		ReplaceActorTransform.SetLocation(LastSelectedPivot);
		//PostCreateNew();
	}

	NewPrefab->PostLoad();

	if (bReplaceActorsWithCreatedPrefab && !NewPrefab->IsEmpty())
	{
		FPrefabToolEditorUtil::ReplaceSelectedActorsAfterPrefabCreated(NewPrefab, ReplaceActorTransform, CommonAttachParentActor, CommonAttachSocket);
	}

	return NewPrefab;
}


bool UPrefabAssetFactory::ShouldShowInNewMenu() const
{
	bool bShouldShow = GEditor->GetSelectedActorCount() > 0;

	// Nested prefab check
	const UPrefabToolSettings* PrefabToolSettings = GetDefault<UPrefabToolSettings>();
	const bool bNestedPrefabSupport = PrefabToolSettings->ShouldEnableNestedPrefabSupport();
	if (!bNestedPrefabSupport)
	{
		TArray<APrefabActor*> SelectedPrefabActors;
		GEditor->GetSelectedActors()->GetSelectedObjects<APrefabActor>(SelectedPrefabActors);
		bShouldShow = SelectedPrefabActors.Num() <= 0;
	}

	return bShouldShow;
}

void UPrefabAssetFactory::PostCreateNew(UObject* PrefabAsset, const FTransform& InTransform)
{
	// Reserve for Post Create Check, currently do nothing
}
