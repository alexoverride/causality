#include "Table.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
	

void URowWidget::SetUp(const int8 RowIndex)
{
	auto BackgroundColor = RowIndex % 2 == 0 ?
		FLinearColor(0.1f, 0.1f, 0.1f, 1.f) :
		FLinearColor(0.2f, 0.2f, 0.2f, 1.f);
	if (RowIndex == 0)
	{
		BackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.f);
	}
	Image = NewObject<UImage>(this);
	Image->SetColorAndOpacity(BackgroundColor);
	
	Background = NewObject<UOverlay>(this);
	Background->AddChildToOverlay(Image);
	const auto ImageSlot = Cast<UOverlaySlot>(Image->Slot);
	ImageSlot->SetHorizontalAlignment(HAlign_Fill);
	ImageSlot->SetVerticalAlignment(VAlign_Fill);

	Container = NewObject<UHorizontalBox>(this);
	Background->AddChildToOverlay(Container);
	const auto ContainerSlot = Cast<UOverlaySlot>(Container->Slot);
	ContainerSlot->SetHorizontalAlignment(HAlign_Fill);
	ContainerSlot->SetPadding(FMargin(2.f));
	
	WidgetTree->RootWidget = Background;
}

void URowWidget::SetNumColumns(const int8 NumColumns)
{
	if (!Container)
	{
		return;
	}
	for (auto i = 0; i < NumColumns; i++)
	{
		const auto Column = NewObject<UTextBlock>(this);
		Columns.Add(Column);
		Container->AddChildToHorizontalBox(Column);
		const auto ColumnSlot = Cast<UHorizontalBoxSlot>(Column->Slot);
		ColumnSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
		ColumnSlot->SetPadding(FMargin(2.f));
	}
}

void URowWidget::SetColumns(const TArray<FText>& NewColumns) const
{
	if (!Container)
	{
		return;
	}
	for (auto i = 0; i < NewColumns.Num(); i++)
	{
		Columns[i]->SetText(NewColumns[i]);
	}
}


void UTableWidget::SetUp()
{
	Container = NewObject<UVerticalBox>(this);
	WidgetTree->RootWidget = Container;
	Title = NewObject<UTextBlock>(this);
	Container->AddChildToVerticalBox(Title);
}

void UTableWidget::SetDimensions(const int8 NumRows, const int8 NumColumns)
{
	// 1 extra row for the header.
	for (auto i = Rows.Num(); i < NumRows + 1; i++)
	{
		auto Row = CreateWidget<URowWidget>(GetWorld());
		Row->SetUp(i);
		Row->SetNumColumns(NumColumns);
		const auto RowSlot = Container->AddChildToVerticalBox(Row);
		RowSlot->SetHorizontalAlignment(HAlign_Fill);
		Rows.Add(Row);
	}
	// 1 extra row for the header.
	for (auto i = 0; i < NumRows + 1; i++)
	{
		Rows[i]->SetVisibility(ESlateVisibility::Visible);
	}
	// 1 extra row for the header.
	for (auto i = NumRows + 1; i < Rows.Num(); i++)
	{
		Rows[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTableWidget::SetTitle(const FText& NewTitle) const
{
	Title->SetText(NewTitle);
}

void UTableWidget::SetRow(const TArray<FText>& NewColumns, const int8 RowIndex)
{
	Rows[RowIndex]->SetColumns(NewColumns);
}
