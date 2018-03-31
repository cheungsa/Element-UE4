// Copyright 2017 marynate. All Rights Reserved.

#include "PrefabComponentDetails.h"
#include "PrefabComponent.h"
#include "PrefabActor.h"
#include "PrefabToolSettings.h"
#include "PrefabToolHelpers.h"
#include "PrefabAsset.h"
#include "CreateBlueprintFromPrefabActorDialog.h"

#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Editor/LevelEditor/Public/LevelEditorActions.h"
#include "ActorEditorUtils.h"
#include "AssetSelection.h"
#include "ScopedTransaction.h"
#include "DetailLayoutBuilder.h"
#include "IDetailsView.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "SlateOptMacros.h"
#include "Engine/Selection.h"

#define LOCTEXT_NAMESPACE "PrefabDetails"

class SPrefabVisibilityWidget : public SImage
{
public:
	SLATE_BEGIN_ARGS(SPrefabVisibilityWidget) {}
	SLATE_END_ARGS()

	/** Construct this widget */
	void Construct(const FArguments& InArgs, TArray<TWeakObjectPtr<APrefabActor>>& InWeakPrefabActors)
	{
		WeakPrefabActors = InWeakPrefabActors;

		SImage::Construct(
			SImage::FArguments()
			.Image(this, &SPrefabVisibilityWidget::GetBrush)
		);
	}

private:

	FReply HandleClick()
	{
		if (WeakPrefabActors.Num() < 1)
		{
			return FReply::Unhandled();
		}

		// Open an undo transaction
		UndoTransaction.Reset(new FScopedTransaction(LOCTEXT("SetActorVisibility", "Set Actor Visibility")));

		const bool bVisible = !IsVisible();

		SetIsVisible(bVisible);

		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}

	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override
	{
		return HandleClick();
	}

	/** Called when the mouse button is pressed down on this widget */
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
		{
			return FReply::Unhandled();
		}

		return HandleClick();
	}

	/** Process a mouse up message */
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			UndoTransaction.Reset();
			return FReply::Handled();
		}

		return FReply::Unhandled();
	}

	/** Called when this widget had captured the mouse, but that capture has been revoked for some reason. */
	virtual void OnMouseCaptureLost() override
	{
		UndoTransaction.Reset();
	}

	/** Get the brush for this widget */
	const FSlateBrush* GetBrush() const
	{
		if (IsVisible())
		{
			return IsHovered() ? FEditorStyle::GetBrush("Level.VisibleHighlightIcon16x") :
				FEditorStyle::GetBrush("Level.VisibleIcon16x");
		}
		else
		{
			return IsHovered() ? FEditorStyle::GetBrush("Level.NotVisibleHighlightIcon16x") :
				FEditorStyle::GetBrush("Level.NotVisibleIcon16x");
		}
	}

	/** Check if the specified item is visible */
	static bool IsVisible(const TArray<TWeakObjectPtr<APrefabActor>>& PrefabActors)
	{
		bool bVisible = true;
		for (const TWeakObjectPtr<APrefabActor>& PrefabActor : PrefabActors)
		{
			if (PrefabActor.IsValid() && PrefabActor->IsTemporarilyHiddenInEditor())
			{
				bVisible = false;
				break;
			}
		}
		return bVisible;
	}

	/** Check if our wrapped tree item is visible */
	bool IsVisible() const
	{
		return IsVisible(WeakPrefabActors);
	}

	/** Set the actor this widget is responsible for to be hidden or shown */
	void SetIsVisible(const bool bVisible)
	{
		struct Local
		{
			static bool IsParentPrefabActorInSelected(const TWeakObjectPtr<APrefabActor>& InActor, const TArray<TWeakObjectPtr<APrefabActor>>& InSelectedActors)
			{
				if (InActor.IsValid() && !InActor->IsPendingKillPending())
				{
					for (AActor* ParentActor = InActor->GetAttachParentActor(); ParentActor && !ParentActor->IsPendingKillPending(); ParentActor = ParentActor->GetAttachParentActor())
					{
						if (ParentActor->IsA(APrefabActor::StaticClass()) && InSelectedActors.Contains(ParentActor))
						{
							return true;
						}
					}
				}
				return false;
			}
		};

		for (const TWeakObjectPtr<APrefabActor>& PrefabActor : WeakPrefabActors)
		{
			if (!Local::IsParentPrefabActorInSelected(PrefabActor, WeakPrefabActors))
			{
				FPrefabActorUtil::ShowHidePrefabActor(PrefabActor.Get(), bVisible);
			}
		}
		GEditor->RedrawAllViewports();
	}

	/** The prefab actor we relate to */
	TArray<TWeakObjectPtr<APrefabActor>> WeakPrefabActors;

	/** Scoped undo transaction */
	TUniquePtr<FScopedTransaction> UndoTransaction;
};

class SPrefabLockSelectionWidget : public SImage
{
public:
	SLATE_BEGIN_ARGS(SPrefabLockSelectionWidget) {}
	SLATE_END_ARGS()

		/** Construct this widget */
		void Construct(const FArguments& InArgs, TArray<TWeakObjectPtr<APrefabActor>>& InWeakPrefabActors)
	{
		WeakPrefabActors = InWeakPrefabActors;

		SImage::Construct(
			SImage::FArguments()
			.Image(this, &SPrefabLockSelectionWidget::GetBrush)
		);
	}

private:

	FReply HandleClick()
	{
		if (WeakPrefabActors.Num() < 1)
		{
			return FReply::Unhandled();
		}

		// Open an undo transaction
		UndoTransaction.Reset(new FScopedTransaction(LOCTEXT("SetPrefabActorLockSelection", "Set Prefab Actor Lock Selection")));

		const bool bLocked = !IsLocked();

		SetLockSelection(bLocked);

		return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
	}

	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override
	{
		return HandleClick();
	}

	/** Called when the mouse button is pressed down on this widget */
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
		{
			return FReply::Unhandled();
		}

		return HandleClick();
	}

	/** Process a mouse up message */
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			UndoTransaction.Reset();
			return FReply::Handled();
		}

		return FReply::Unhandled();
	}

	/** Called when this widget had captured the mouse, but that capture has been revoked for some reason. */
	virtual void OnMouseCaptureLost() override
	{
		UndoTransaction.Reset();
	}

	/** Get the brush for this widget */
	const FSlateBrush* GetBrush() const
	{
		if (IsLocked())
		{
			return IsHovered() ? FEditorStyle::GetBrush("CurveEd.LockedHighlight") :
				FEditorStyle::GetBrush("CurveEd.Locked");
		}
		else
		{
			return IsHovered() ? FEditorStyle::GetBrush("CurveEd.Unlocked") :
				FEditorStyle::GetBrush("CurveEd.UnlockedHighlight");
		}
	}

	/** Check if the specified item is visible */
	static bool IsLocked(const TArray<TWeakObjectPtr<APrefabActor>>& PrefabActors)
	{
		bool bLocked = false;
		for (const TWeakObjectPtr<APrefabActor>& PrefabActor : PrefabActors)
		{
			if (PrefabActor.IsValid() && PrefabActor->GetLockSelection())
			{
				bLocked = true;
				break;
			}
		}
		return bLocked;
	}

	/** Check if our wrapped tree item is visible */
	bool IsLocked() const
	{
		return IsLocked(WeakPrefabActors);
	}

	/** Set the actor this widget is responsible for to be hidden or shown */
	void SetLockSelection(const bool bLocked)
	{
		struct Local
		{
			static bool IsParentPrefabActorInSelected(const TWeakObjectPtr<APrefabActor>& InActor, const TArray<TWeakObjectPtr<APrefabActor>>& InSelectedActors)
			{
				if (InActor.IsValid() && !InActor->IsPendingKillPending())
				{
					for (AActor* ParentActor = InActor->GetAttachParentActor(); ParentActor && !ParentActor->IsPendingKillPending(); ParentActor = ParentActor->GetAttachParentActor())
					{
						if (ParentActor->IsA(APrefabActor::StaticClass()) && InSelectedActors.Contains(ParentActor))
						{
							return true;
						}
					}
				}
				return false;
			}
		};

		for (const TWeakObjectPtr<APrefabActor>& PrefabActor : WeakPrefabActors)
		{
// 			if (!Local::IsParentPrefabActorInSelected(PrefabActor, WeakPrefabActors))
// 			{
// 				PrefabActor->SetLockSelection(bLocked, /*bCheckParent=*/ true, /*bRecursive*/ true);
// 			}
			PrefabActor->SetLockSelection(bLocked);
		}
	}

	/** The prefab actor we relate to */
	TArray<TWeakObjectPtr<APrefabActor>> WeakPrefabActors;

	/** Scoped undo transaction */
	TUniquePtr<FScopedTransaction> UndoTransaction;
};

TSharedRef<IDetailCustomization> FPrefabComponentDetails::MakeInstance()
{
	return MakeShareable( new FPrefabComponentDetails );
}

FPrefabComponentDetails::~FPrefabComponentDetails()
{
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void FPrefabComponentDetails::CustomizeDetails( IDetailLayoutBuilder& DetailLayout )
{
	SelectedPrefabActors.Empty();
	TArray< TWeakObjectPtr<UObject> > SelectedObjects = DetailLayout.GetDetailsView()->GetSelectedObjects();
	for (int32 ObjIdx = 0; ObjIdx < SelectedObjects.Num(); ObjIdx++)
	{
		if (APrefabActor* PrefabActor = Cast<APrefabActor>(SelectedObjects[ObjIdx].Get()))
		{
			SelectedPrefabActors.Add(PrefabActor);
		}
	}

 	IDetailCategoryBuilder& PrefabCategory = DetailLayout.EditCategory( "Prefab", FText::GetEmpty());
 
 	TSharedPtr<SHorizontalBox> PrefabHorizontalBox;

	PrefabCategory.AddCustomRow(FText::GetEmpty(), false)
	[
		SAssignNew(PrefabHorizontalBox, SHorizontalBox)
	];

	PrefabHorizontalBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.Padding(0, 0, 4, 0)
	[
		SNew(SPrefabVisibilityWidget, SelectedPrefabActors)
	];

	PrefabHorizontalBox->AddSlot()
	.AutoWidth()
	.VAlign(VAlign_Center)
	.Padding(0, 0, 4, 0)
	[
		SNew(SPrefabLockSelectionWidget, SelectedPrefabActors)
	];

	PrefabHorizontalBox->AddSlot()
	.AutoWidth()
	.Padding(0, 0, 2, 0)
	[
		SNew(SProperty, DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UPrefabComponent, bConnected)))
		.ShouldDisplayName(false)
	];

	PrefabHorizontalBox->AddSlot()
	.AutoWidth()
	.Padding(0, 0, 6, 0)
	.VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Font(DetailLayout.GetDetailFont())
		.Text(LOCTEXT("Connected", "Connected"))
		.IsEnabled(this, &FPrefabComponentDetails::HasConnectedPrefab)
	];

	PrefabHorizontalBox->AddSlot()
	[
		SNew(SButton)
		.ToolTipText(LOCTEXT("SelectPrefab_Tooltip", "Select Prefab Asset in Content Browser"))
		.OnClicked(this, &FPrefabComponentDetails::OnSelectPrefabClicked)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Select", "Select"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
	];

	PrefabHorizontalBox->AddSlot()
	[
		SNew(SButton)
		.ToolTipText(LOCTEXT("RevertPrefab_Tooltip", "Revert Prefab Instances to Original Prefab Asset"))
		.OnClicked(this, &FPrefabComponentDetails::OnRevertPrefabClicked)
		.IsEnabled(this, &FPrefabComponentDetails::HasConnectedPrefab)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Revert", "Revert"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
	];

	PrefabHorizontalBox->AddSlot()
	[
		SNew(SButton)
		.ToolTipText(LOCTEXT("ApplyPrefab_Tooltip", "Apply Prefab Instances Changes Back to Prefab Asset"))
		.OnClicked(this, &FPrefabComponentDetails::OnApplyPrefabClicked)
		.IsEnabled(this, &FPrefabComponentDetails::HasConnectedPrefab)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Apply", "Apply"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
	];

	PrefabCategory.AddCustomRow(FText::GetEmpty(), false)
	[
		SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		//.Padding(0, 0, 2, 0)
		[
			SNew(SButton)
			.ToolTipText(LOCTEXT("ConvertToNormalActor_Tooltip", "Convert Prefab Actor to Normal Actor"))
			.OnClicked(this, &FPrefabComponentDetails::OnConvertPrefabToNormalActorClicked)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ConvertToNormal", "Convert to Normal"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		]

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		//.Padding(0, 0, 2, 0)
		[
			SNew(SButton)
			.ToolTipText(LOCTEXT("DestroyPrefabActor_Tooltip", "Destroy Prefab Actor Only"))
			.OnClicked(this, &FPrefabComponentDetails::OnDestroyPrefabActorClicked)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Destroy", "Destroy"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		]

		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		//.Padding(0, 0, 2, 0)
		[
			SNew(SButton)
			.ToolTipText(LOCTEXT("DestroyPrefabActorHierarchy_Tooltip", "Destroy Prefab Actor and All Attached Children Actors"))
			.OnClicked(this, &FPrefabComponentDetails::OnDestroyPrefabActorHierarchyClicked)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("DestroyHierarchy", "Destroy Hierarchy"))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		]
	];

	const bool bSupportBlueprintGeneration = GetDefault<UPrefabToolSettings>()->ShouldEnablebBlueprintGenerationSupport();
	if (bSupportBlueprintGeneration)
	{
		// Convert to Blueprint
		PrefabCategory.AddCustomRow(FText::GetEmpty(), false)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			//.Padding(0, 0, 2, 0)
			[
				SNew(SButton)
				.ToolTipText(LOCTEXT("GenerateBlueprint_Tooltip", "Generate Blueprint from Prefab Actor"))
				.OnClicked(this, &FPrefabComponentDetails::OnGenerateBlueprintClicked)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("GenerateBlueprint", "Generate Blueprint"))
					.Font(IDetailLayoutBuilder::GetDetailFont())
				]
			]

			+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			//.Padding(0, 0, 2, 0)
			[
				SNew(SButton)
				.ToolTipText(LOCTEXT("UpdateBlueprint_Tooltip", "Update Blueprint from Prefab Actor"))
				.OnClicked(this, &FPrefabComponentDetails::OnUpdateBlueprintClicked)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("UpdateBlueprint", "Update Blueprint"))
					.Font(IDetailLayoutBuilder::GetDetailFont())
				]
			]
		];
	}
	if (!bSupportBlueprintGeneration)
	{
		DetailLayout.HideProperty(GET_MEMBER_NAME_CHECKED(UPrefabComponent, GeneratedBlueprint));
	}

	const bool bDebugMode = GetDefault<UPrefabToolSettings>()->IsDebugMode();
	if (!bDebugMode)
	{
		DetailLayout.HideProperty(GET_MEMBER_NAME_CHECKED(UPrefabComponent, PrefabInstancesMap));
	}

	DetailLayout.HideProperty(GET_MEMBER_NAME_CHECKED(UPrefabComponent, bLockSelection));
	DetailLayout.HideProperty(GET_MEMBER_NAME_CHECKED(UPrefabComponent, bTransient));
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply FPrefabComponentDetails::OnSelectPrefabClicked()
{
	if (SelectedPrefabActors.Num() > 0)
	{
		TArray<FAssetData> AssetDataList;

		TArray<APrefabActor*> ValidSelectedPrefabActors;
		CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);
		for (APrefabActor* PrefabActor : ValidSelectedPrefabActors)
		{
			UPrefabAsset* Prefab = PrefabActor->GetPrefabComponent()->GetPrefab();
			if (Prefab)
			{
				AssetDataList.Emplace(Prefab);
			}
		}
		GEditor->SyncBrowserToObjects(AssetDataList);
	}
	return FReply::Handled();
}

FReply FPrefabComponentDetails::OnApplyPrefabClicked()
{
	const FScopedTransaction Transaction(LOCTEXT("ApplyToPrefabAsset", "Apply to Prefab Asset"));

	TArray<APrefabActor*> ValidSelectedPrefabActors;
	CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);

	for (APrefabActor* PrefabActor : ValidSelectedPrefabActors)
	{
		FPrefabToolEditorUtil::ApplyPrefabActor(PrefabActor);
	}

	FPrefabToolEditorUtil::BeginSkipSelectionMonitor();
	{
		GEditor->SelectNone(/*bNoteSelectionChange*/false, true);
		for (AActor* PrefabActor : ValidSelectedPrefabActors)
		{
			GEditor->SelectActor(PrefabActor, /*bInSelected=*/true, /*bNotify=*/ false, /*bSelectEvenIfHidden=*/ true);
		}
		GEditor->NoteSelectionChange();
	}
	FPrefabToolEditorUtil::EndSkipSelectionMonitor();

	return FReply::Handled();
}

FReply FPrefabComponentDetails::OnConvertPrefabToNormalActorClicked()
{
	const FScopedTransaction Transaction(LOCTEXT("ConvertPrefabActor", "Convert Prefab Actor to Normal Actor"));

	TArray<APrefabActor*> ValidSelectedPrefabActors;
	CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);

	USelection* SelectedActors = GEditor->GetSelectedActors();
	SelectedActors->BeginBatchSelectOperation();
	SelectedActors->Modify();

	struct Local
	{
		static bool IsParentPrefabActorInSelected(AActor* InActor, const TArray<APrefabActor*>& InSelectedActors)
		{
			if (InActor && !InActor->IsPendingKillPending())
			{
				for (AActor* ParentActor = InActor->GetAttachParentActor(); ParentActor && !ParentActor->IsPendingKillPending(); ParentActor = ParentActor->GetAttachParentActor())
				{
					if (ParentActor->IsA(APrefabActor::StaticClass()) && InSelectedActors.Contains(ParentActor))
					{
						return true;
					}
				}
			}
			return false;
		}
	};

	for (int32 Index = ValidSelectedPrefabActors.Num() - 1; Index >= 0; --Index)
	{
		AActor* Actor = ValidSelectedPrefabActors[Index];
		if (Local::IsParentPrefabActorInSelected(Actor, ValidSelectedPrefabActors))
		{
			ValidSelectedPrefabActors.RemoveAt(Index);
		}
	}

	for (APrefabActor* PrefabActor : ValidSelectedPrefabActors)
	{
		FPrefabToolEditorUtil::ConvertPrefabActorToNormalActor(PrefabActor);
	}

	GEditor->NoteSelectionChange();

	return FReply::Handled();
}

FReply FPrefabComponentDetails::OnDestroyPrefabActorClicked()
{
	DoDestroySelectedPrefabActor(/*bDeleteInstanceActors=*/ false);

	return FReply::Handled();
}

FReply FPrefabComponentDetails::OnDestroyPrefabActorHierarchyClicked()
{
	DoDestroySelectedPrefabActor(/*bDeleteInstanceActors=*/ true);

	return FReply::Handled();
}

FReply FPrefabComponentDetails::OnGenerateBlueprintClicked()
{
	const FScopedTransaction Transaction(LOCTEXT("GenerateBlueprintFromPrefabActor", "Generate Blueprint From Prefab Actor"));

	TArray<APrefabActor*> ValidSelectedPrefabActors;
	CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);

	if (ValidSelectedPrefabActors.Num() == 1)
	{
		FCreateBlueprintFromPrefabActorDialog::OpenDialog(ValidSelectedPrefabActors[0]);
	}

	return FReply::Handled();
}

FReply FPrefabComponentDetails::OnUpdateBlueprintClicked()
{
	const FScopedTransaction Transaction(LOCTEXT("UpdateBlueprintFromPrefabActor", "Update Blueprint From Prefab Actor"));

	TArray<APrefabActor*> ValidSelectedPrefabActors;
	CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);

	if (ValidSelectedPrefabActors.Num() == 1)
	{
		APrefabActor* PrefabActor = ValidSelectedPrefabActors[0];
		if (UBlueprint* ExistingBlueprint = PrefabActor->GetPrefabComponent()->GeneratedBlueprint)
		{
			//if (UBlueprint* BlueprintAsset = Cast<UBlueprint>(LoadObject<UBlueprint>(nullptr, *InAssetPath)))
			FCreateBlueprintFromPrefabActor Creator;
			const bool bReplaceActor = true;
			Creator.UpdateBlueprint(ExistingBlueprint, PrefabActor, bReplaceActor);
		}
	}

	return FReply::Handled();
}

void FPrefabComponentDetails::DoDestroySelectedPrefabActor(bool bDeleteInstanceActors)
{
	TArray<APrefabActor*> ValidSelectedPrefabActors;
	CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);

	if (ValidSelectedPrefabActors.Num() > 0)
	{
		const FScopedTransaction Transaction(LOCTEXT("DestroyPrefabActors", "Destroy Prefab Actors"));

		GEditor->SelectNone(/*bNoteSelectionChange=*/ true, /*bDeselectBSPSurfs=*/ true, /*WarnAboutManyActors=*/ false);

		if (bDeleteInstanceActors)
		{
			struct Local
			{
				static bool IsParentPrefabActorInSelected(AActor* InActor, const TArray<APrefabActor*>& InSelectedActors)
				{
					if (InActor && !InActor->IsPendingKillPending())
					{
						for (AActor* ParentActor = InActor->GetAttachParentActor(); ParentActor && !ParentActor->IsPendingKillPending(); ParentActor = ParentActor->GetAttachParentActor())
						{
							if (ParentActor->IsA(APrefabActor::StaticClass()) && InSelectedActors.Contains(ParentActor))
							{
								return true;
							}
						}
					}
					return false;
				}
			};

			for (int32 Index = ValidSelectedPrefabActors.Num() - 1; Index >= 0; --Index)
			{
				AActor* Actor = ValidSelectedPrefabActors[Index];
				if (Local::IsParentPrefabActorInSelected(Actor, ValidSelectedPrefabActors))
				{
					ValidSelectedPrefabActors.RemoveAt(Index);
				}
			}
		}

		for (APrefabActor* PrefabActor : ValidSelectedPrefabActors)
		{
			FPrefabToolEditorUtil::DestroyPrefabActor(PrefabActor, bDeleteInstanceActors);
		}
	}
}

FReply FPrefabComponentDetails::OnRevertPrefabClicked()
{
	const FScopedTransaction Transaction(LOCTEXT("RevertPrefabActors", "Revert Prefab Actors"));

	TArray<APrefabActor*> ValidSelectedPrefabActors;
	CopyFromWeakArray(ValidSelectedPrefabActors, SelectedPrefabActors);
	for (APrefabActor* PrefabActor : ValidSelectedPrefabActors)
	{
		FPrefabToolEditorUtil::RevertPrefabActor(PrefabActor);
	}
	return FReply::Handled();
}

bool FPrefabComponentDetails::HasConnectedPrefab() const
{
	bool bHasConnectedPrefab = false;
	if (SelectedPrefabActors.Num() > 0)
	{
		for (int32 ObjIdx = 0; ObjIdx < SelectedPrefabActors.Num(); ObjIdx++)
		{
			if (APrefabActor* PrefabActor = SelectedPrefabActors[ObjIdx].Get())
			{
				if (PrefabActor->GetPrefabComponent() && PrefabActor->GetPrefabComponent()->GetConnected())
				{
					bHasConnectedPrefab = true;
					break;
				}
			}
		}
	}
	return bHasConnectedPrefab;
}

#undef LOCTEXT_NAMESPACE
