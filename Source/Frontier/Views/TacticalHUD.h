#pragma once

#include "GameFramework/HUD.h"
#include "Widgets/Layout/Anchors.h"
#include "TacticalHUD.generated.h"

class UCanvasPanel;
class UUnitTable;
class UTeamsTable;
class UActionTable;

UCLASS()
class ATacticalHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void DrawHUD() override;

	template <typename WidgetType>
	WidgetType* CreateAndPositionWidget(FVector2D Size, FAnchors Anchors);

private:
	UPROPERTY()
	TObjectPtr<UCanvasPanel> CanvasPanel;
	UPROPERTY()
	TObjectPtr<UUnitTable> UnitTable;
	UPROPERTY()
	TObjectPtr<UTeamsTable> TeamsTable;
	UPROPERTY()
	TObjectPtr<UActionTable> ActionTable;
};
