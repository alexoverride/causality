#include "TacticalHUD.h"
#include "ActionTable.h"
#include "TeamsTable.h"
#include "UnitTable.h"
#include "Blueprint/UserWidget.h"


void ATacticalHUD::BeginPlay()
{
	Super::BeginPlay();

	UnitTable = CreateAndPositionWidget<UUnitTable>(
		FVector2D(400.0f, 300.0f),
		FAnchors(0.05f, 0.05f, 0.05f, 0.05f)
	);

	TeamsTable = CreateAndPositionWidget<UTeamsTable>(
		FVector2D(400.0f, 300.0f),
		FAnchors(0.05f, 0.9f, 0.05f, 0.9f)
	);

	ActionTable = CreateAndPositionWidget<UActionTable>(
		FVector2D(600.0f, 300.0f),
		FAnchors(0.8f, 0.05f, 0.8f, 0.05f)
	);
}

void ATacticalHUD::DrawHUD()
{
	Super::DrawHUD();

	UnitTable->Populate();
	TeamsTable->Populate();
	ActionTable->Populate();
}

template <typename WidgetType>
WidgetType* ATacticalHUD::CreateAndPositionWidget(FVector2D Size, FAnchors Anchors)
{
	auto Widget = CreateWidget<WidgetType>(GetWorld());
	Widget->SetUp(GetWorld());
	Widget->AddToViewport();

	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FVector2D Position = FVector2D(
		ViewportSize.X * Anchors.Minimum.X,
		ViewportSize.Y * Anchors.Minimum.Y
	);

	Widget->SetDesiredSizeInViewport(Size);
	Widget->SetAlignmentInViewport(FVector2D(Anchors.Minimum.X, Anchors.Minimum.Y));
	Widget->SetPositionInViewport(Position);

	return Widget;
}
