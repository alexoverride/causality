#include "UnitTable.h"
#include "Tactical/TacticalTurn.h"
#include "Tactical/TacticalGameMode.h"
#include "Table.h"
#include "Blueprint/WidgetTree.h"
#include "TwinStick/Unit.h"


void UUnitTable::SetUp(UWorld* InWorld)
{
    GameMode = Cast<ATacticalGameMode>(GetWorld()->GetAuthGameMode());

    TableWidget = CreateWidget<UTableWidget>(InWorld);
    WidgetTree->RootWidget = TableWidget;
    TableWidget->SetUp();
}

void UUnitTable::Populate() const
{
    const auto CurrentTeam = GameMode->Teams[GameMode->InitiativeIndex];

    if (const auto CurrentUnit = CurrentTeam->GetCurrentUnit())
    {
        constexpr int32 NumColumns = 2;
        TArray<FText> Row; // reusable alloc

        TableWidget->SetTitle(FText::FromString("Current Unit"));
        TableWidget->SetDimensions(3, NumColumns);

        const TArray<FText> Headers = {
            FText::FromString("Property"),
            FText::FromString("Value")
        };
        TableWidget->SetRow(Headers, 0);

        Row.Empty();
        Row.Add(FText::FromString("Unit Name"));
        Row.Add(FText::FromString(CurrentUnit->GetName()));
        TableWidget->SetRow(Row, 1);

        Row.Empty();
        Row.Add(FText::FromString("Actions"));
        Row.Add(FText::FromString(FString::FromInt(CurrentUnit->Actions)));
        TableWidget->SetRow(Row, 2);

        Row.Empty();
        Row.Add(FText::FromString("Reaction"));
        Row.Add(FText::FromString(FString::FromInt(CurrentUnit->Reaction)));
        TableWidget->SetRow(Row, 3);
    }
}
