#include "VoxelWorldDetails.h"
#include "VoxelWorld.h"
#include "VoxelWorldEditor.h"
#include "PropertyHandle.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailCategoryBuilder.h"
#include "IDetailsView.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "LevelEditorViewport.h"
#include "Editor.h"
#include "IDetailPropertyRow.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY(VoxelEditorLog)

FVoxelWorldDetails::FVoxelWorldDetails()
{

}

FVoxelWorldDetails::~FVoxelWorldDetails()
{

}

TSharedRef<IDetailCustomization> FVoxelWorldDetails::MakeInstance()
{
	return MakeShareable(new FVoxelWorldDetails());
}

void FVoxelWorldDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	const TArray< TWeakObjectPtr<UObject> >& SelectedObjects = DetailLayout.GetDetailsView()->GetSelectedObjects();

	for (int32 ObjectIndex = 0; ObjectIndex < SelectedObjects.Num(); ++ObjectIndex)
	{
		const TWeakObjectPtr<UObject>& CurrentObject = SelectedObjects[ObjectIndex];
		if (CurrentObject.IsValid())
		{
			AVoxelWorld* CurrentCaptureActor = Cast<AVoxelWorld>(CurrentObject.Get());
			if (CurrentCaptureActor)
			{
				World = CurrentCaptureActor;
				break;
			}
		}
	}

	DetailLayout.EditCategory("Voxel")
		.AddCustomRow(FText::FromString(TEXT("Update")))
		.NameContent()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
		.Text(FText::FromString(TEXT("Toggle world preview")))
		]
	.ValueContent()
		.MaxDesiredWidth(125.f)
		.MinDesiredWidth(125.f)
		[
			SNew(SButton)
			.ContentPadding(2)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.OnClicked(this, &FVoxelWorldDetails::OnWorldPreviewToggle)
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
		.Text(FText::FromString(TEXT("Toggle")))
		]
		];
}

FReply FVoxelWorldDetails::OnWorldPreviewToggle()
{
	if (World.IsValid())
	{
		World->VoxelWorldEditorClass = AVoxelWorldEditor::StaticClass();

		if (World->IsCreated())
		{
			World->DestroyInEditor();
		}
		else
		{
			World->CreateInEditor();
		}
	}

	return FReply::Handled();
}