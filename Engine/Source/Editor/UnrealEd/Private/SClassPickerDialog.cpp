// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "UnrealEd.h"
#include "Editor/ClassViewer/Private/SClassViewer.h"
#include "Editor/UnrealEd/Classes/Settings/EditorExperimentalSettings.h"
#include "SClassPickerDialog.h"
#include "EditorClassUtils.h"
#include "SExpandableArea.h"
#include "ClassIconFinder.h"

#define LOCTEXT_NAMESPACE "SClassPicker"

void SClassPickerDialog::Construct(const FArguments& InArgs)
{
	WeakParentWindow = InArgs._ParentWindow;

	bPressedOk = false;
	ChosenClass = NULL;

	ClassViewer = StaticCastSharedRef<SClassViewer>(FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(InArgs._Options, FOnClassPicked::CreateSP(this,&SClassPickerDialog::OnClassPicked)));

	// Determine if components are allowed
	const UEditorExperimentalSettings* ExperimentalSettings = GetDefault<UEditorExperimentalSettings>();
	const bool bForBlueprint = (InArgs._AssetType != nullptr) && (InArgs._AssetType->IsChildOf(UBlueprint::StaticClass()));
	const bool bAllowComponentSubclasses = !bForBlueprint || ExperimentalSettings->bBlueprintableComponents;

	// Load in default settings
	for (const FClassPickerDefaults& DefaultObj : GUnrealEd->GetUnrealEdOptions()->NewAssetDefaultClasses)
	{
		UClass* AssetType = LoadClass<UObject>(NULL, *DefaultObj.AssetClass, NULL, LOAD_None, NULL);
		UClass* AssetActualClass = LoadClass<UObject>(NULL, *DefaultObj.ClassName, NULL, LOAD_None, NULL);
		const bool bIsComponent = AssetActualClass->IsChildOf(UActorComponent::StaticClass());

		if (InArgs._AssetType->IsChildOf(AssetType) && (!bIsComponent || bAllowComponentSubclasses))
		{
			AssetDefaultClasses.Add(MakeShareable(new FClassPickerDefaults(DefaultObj)));
		}
	}

	const bool bHasDefaultClasses = AssetDefaultClasses.Num() > 0;

	bool bExpandDefaultClassPicker = true;
	bool bExpandCustomClassPicker = !bHasDefaultClasses;

	if (bHasDefaultClasses)
	{
		GConfig->GetBool(TEXT("/Script/UnrealEd.UnrealEdOptions"), TEXT("bExpandClassPickerDefaultClassList"), bExpandDefaultClassPicker, GEditorIni);
		GConfig->GetBool(TEXT("/Script/UnrealEd.UnrealEdOptions"), TEXT("bExpandCustomClassPickerClassList"), bExpandCustomClassPicker, GEditorIni);
	}

	ChildSlot
	[
		SNew(SBorder)
		.Visibility(EVisibility::Visible)
		.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		[
			SNew(SBox)
			.Visibility(EVisibility::Visible)
			.WidthOverride(520.0f)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SExpandableArea)
					.InitiallyCollapsed(!bExpandDefaultClassPicker)
					.AreaTitle(NSLOCTEXT("SClassPickerDialog", "CommonClassesAreaTitle", "Common Classes"))
					.OnAreaExpansionChanged(this, &SClassPickerDialog::OnDefaultAreaExpansionChanged)
					.BodyContent()
					[
						SNew(SListView < TSharedPtr<FClassPickerDefaults>  >)
						.ItemHeight(24)
						.SelectionMode(ESelectionMode::None)
						.ListItemsSource(&AssetDefaultClasses)
						.OnGenerateRow(this, &SClassPickerDialog::GenerateListRow)
						.Visibility(bHasDefaultClasses? EVisibility::Visible: EVisibility::Collapsed)
					]
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 10.0f, 0.0f, 0.0f)
				[
					SNew(SExpandableArea)
					.MaxHeight(320.f)
					.InitiallyCollapsed(!bExpandCustomClassPicker)
					.AreaTitle(NSLOCTEXT("SClassPickerDialog", "AllClassesAreaTitle", "All Classes"))
					.OnAreaExpansionChanged(this, &SClassPickerDialog::OnCustomAreaExpansionChanged)
					.BodyContent()
					[
						ClassViewer.ToSharedRef()
					]
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				.Padding(8)
				[
					SNew(SUniformGridPanel)
					.SlotPadding(FEditorStyle::GetMargin("StandardDialog.SlotPadding"))
					+SUniformGridPanel::Slot(0,0)
					[
						SNew(SButton)
						.Text(NSLOCTEXT("SClassPickerDialog", "ClassPickerSelectButton", "Select"))
						.HAlign(HAlign_Center)
						.Visibility( this, &SClassPickerDialog::GetSelectButtonVisibility )
						.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
						.OnClicked(this, &SClassPickerDialog::OnClassPickerConfirmed)
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
						.TextStyle(FEditorStyle::Get(), "FlatButton.DefaultTextStyle")
					]
					+SUniformGridPanel::Slot(1,0)
					[
						SNew(SButton)
						.Text(NSLOCTEXT("SClassPickerDialog", "ClassPickerCancelButton", "Cancel"))
						.HAlign(HAlign_Center)
						.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
						.OnClicked(this, &SClassPickerDialog::OnClassPickerCanceled)
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Default")
						.TextStyle(FEditorStyle::Get(), "FlatButton.DefaultTextStyle")
					]
				]
			]
		]
	];
}

bool SClassPickerDialog::PickClass(const FText& TitleText, const FClassViewerInitializationOptions& ClassViewerOptions, UClass*& OutChosenClass, UClass* AssetType)
{
	// Create the window to pick the class
	TSharedRef<SWindow> PickerWindow = SNew(SWindow)
		.Title(TitleText)
		.SizingRule( ESizingRule::Autosized )
		.ClientSize( FVector2D( 0.f, 300.f ))
		.SupportsMaximize(false)
		.SupportsMinimize(false);

	TSharedRef<SClassPickerDialog> ClassPickerDialog = SNew(SClassPickerDialog)
		.ParentWindow(PickerWindow)
		.Options(ClassViewerOptions)
		.AssetType(AssetType);

	PickerWindow->SetContent(ClassPickerDialog);

	GEditor->EditorAddModalWindow(PickerWindow);

	if (ClassPickerDialog->bPressedOk)
	{
		OutChosenClass = ClassPickerDialog->ChosenClass;
		return true;
	}
	else
	{
		// Ok was not selected, NULL the class
		OutChosenClass = NULL;
		return false;
	}
}

void SClassPickerDialog::OnClassPicked(UClass* InChosenClass)
{
	ChosenClass = InChosenClass;
}

TSharedRef<ITableRow> SClassPickerDialog::GenerateListRow(TSharedPtr<FClassPickerDefaults> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	FClassPickerDefaults* Obj = InItem.Get();
	UClass* ItemClass = LoadClass<UObject>(NULL, *Obj->ClassName, NULL, LOAD_None, NULL);
	const FSlateBrush* ItemBrush = FClassIconFinder::FindIconForClass(ItemClass);

	return 
	SNew(STableRow< TSharedPtr<FClassPickerDefaults> >, OwnerTable)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.MaxHeight(30.0f)
		.Padding(10.0f, 6.0f, 0.0f, 4.0f)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(0.45f)
			[
				SNew(SButton)
				.OnClicked(this, &SClassPickerDialog::OnDefaultClassPicked, ItemClass)
				.ToolTip(FEditorClassUtils::GetTooltip(ItemClass))
				.Content()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.FillWidth(0.12f)
					[
						SNew(SImage)
						.Image(ItemBrush)
					]
					+SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.Padding(4.0f, 0.0f)
					.FillWidth(0.8f)
					[
						SNew(STextBlock)
						.Text(Obj->GetName())
					]

				]
			]
			+SHorizontalBox::Slot()
			.Padding(10.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(Obj->GetDescription())
				.AutoWrapText(true)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				FEditorClassUtils::GetDocumentationLinkWidget(ItemClass)
			]
		]
	];
}

FReply SClassPickerDialog::OnDefaultClassPicked(UClass* InChosenClass)
{
	ChosenClass = InChosenClass;
	bPressedOk = true;
	if (WeakParentWindow.IsValid())
	{
		WeakParentWindow.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}

FReply SClassPickerDialog::OnClassPickerConfirmed()
{
	if (ChosenClass == NULL)
	{
		FMessageDialog::Open(EAppMsgType::Ok, NSLOCTEXT("EditorFactories", "MustChooseClassWarning", "You must choose a class."));
	}
	else
	{
		bPressedOk = true;

		if (WeakParentWindow.IsValid())
		{
			WeakParentWindow.Pin()->RequestDestroyWindow();
		}
	}
	return FReply::Handled();
}

FReply SClassPickerDialog::OnClassPickerCanceled()
{
	if (WeakParentWindow.IsValid())
	{
		WeakParentWindow.Pin()->RequestDestroyWindow();
	}
	return FReply::Handled();
}

void SClassPickerDialog::OnDefaultAreaExpansionChanged(bool bExpanded)
{
	if (bExpanded && WeakParentWindow.IsValid())
	{
		WeakParentWindow.Pin().Get()->SetWidgetToFocusOnActivate(ClassViewer);
	}

	if (AssetDefaultClasses.Num() > 0)
	{
		GConfig->SetBool(TEXT("/Script/UnrealEd.UnrealEdOptions"), TEXT("bExpandClassPickerDefaultClassList"), bExpanded, GEditorIni);
	}
}

void SClassPickerDialog::OnCustomAreaExpansionChanged(bool bExpanded)
{
	if (bExpanded && WeakParentWindow.IsValid())
	{
		WeakParentWindow.Pin().Get()->SetWidgetToFocusOnActivate(ClassViewer);
	}

	if (AssetDefaultClasses.Num() > 0)
	{
		GConfig->SetBool(TEXT("/Script/UnrealEd.UnrealEdOptions"), TEXT("bExpandCustomClassPickerClassList"), bExpanded, GEditorIni);
	}
}

EVisibility SClassPickerDialog::GetSelectButtonVisibility() const
{
	EVisibility Visibility = EVisibility::Hidden;
	if( ChosenClass != NULL )
	{
		Visibility = EVisibility::Visible;
	}
	return Visibility;
}

/** Overridden from SWidget: Called when a key is pressed down - capturing copy */
FReply SClassPickerDialog::OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent )
{
	WeakParentWindow.Pin().Get()->SetWidgetToFocusOnActivate(ClassViewer);

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		return OnClassPickerCanceled();
	}
	else
	{
		return ClassViewer->OnKeyDown(MyGeometry, InKeyEvent);
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
