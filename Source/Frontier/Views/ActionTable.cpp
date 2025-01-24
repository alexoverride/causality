#include "ActionTable.h"
#include "Tactical/TacticalGameMode.h"
#include "Table.h"
#include "Action/BaseClasses.h"
#include "Blueprint/WidgetTree.h"
#include "Relations/CausalityLattice.h"
#include "TwinStick/Unit.h"

void UActionTable::SetUp(UWorld* InWorld)
{
	GameMode = Cast<ATacticalGameMode>(GetWorld()->GetAuthGameMode());

	TableWidget = CreateWidget<UTableWidget>(InWorld);
	WidgetTree->RootWidget = TableWidget;
	TableWidget->SetUp();
}

void UActionTable::Populate() const
{
	const auto Actions = GameMode->CausalityLattice->GetActions();
	const auto NextTrigger = GameMode->CausalityLattice->GetNextTrigger();

	constexpr int32 MaxRows = 20;
	constexpr int32 NumColumns = 4;

	TableWidget->SetTitle(FText::FromString("Causality Lattice Actions"));
	TableWidget->SetDimensions(FMath::Min(Actions.Num(), MaxRows), NumColumns);

	const TArray<FText> Headers = {
		FText::FromString("Name"),
		FText::FromString("Trigger"),
		FText::FromString("Source"),
		FText::FromString("Endable")
	};
	TableWidget->SetRow(Headers, 0);

	const int32 StartIndex = FMath::Max(Actions.Num() - MaxRows, 0);
	for (int32 i = StartIndex; i < Actions.Num(); ++i)
	{
		const auto Action = Actions[i];
		TArray<FText> Row;

		Row.Add(FText::FromString(Action->GetName()));
		Row.Add(FText::FromString(Action->Trigger ? Action->Trigger->GetName() : TEXT("")));
		Row.Add(FText::FromString(Action->Source->GetName()));
		Row.Add(FText::FromString(Action == NextTrigger ? TEXT("Yes") : TEXT("")));

		TableWidget->SetRow(Row, i - StartIndex + 1);
	}
}
