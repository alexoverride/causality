#include "TeamsTable.h"
#include "Blueprint/WidgetTree.h"
#include "Tactical/TacticalGameMode.h"
#include "Tactical/TacticalPlayerController.h"
#include "Tactical/TacticalTurn.h"


void UTeamsTable::SetUp(UWorld* InWorld)
{
	GameMode = Cast<ATacticalGameMode>(InWorld->GetAuthGameMode());
	TableWidget = CreateWidget<UTableWidget>(InWorld);
	WidgetTree->RootWidget = TableWidget;
	TableWidget->SetUp();
}

void UTeamsTable::Populate() const
{
	const auto CurrentTeam = GameMode->Teams[GameMode->InitiativeIndex];
	const int32 NumTeams = GameMode->Teams.Num();
	TableWidget->SetDimensions(NumTeams, 3);
	TableWidget->SetTitle(FText::FromString("Teams"));
	TArray<FText> Row; // reuse alloc

	{
		const TArray<FText> Header = {
			FText::FromString("Team"),
			FText::FromString("Decision"),
			FText::FromString("Initiative")
		};
		TableWidget->SetRow(Header, 0);
	}

	auto PlayerNum = 1;
	auto AINum = 1;
	for (int32 i = 0; i < NumTeams; ++i)
	{
		Row.Empty();
		const auto Team = GameMode->Teams[i];

		// Column 1:  Team name
		const auto RawTeamName = Cast<AController>(Team)->GetName();
		FString TeamName;
		if (Cast<ATacticalPlayerController>(Team))
		{
			const auto TeamType = TEXT("Player");
			TeamName = FString::Printf(TEXT("%s %d"), TeamType, PlayerNum++);
		}
		else
		{
			const auto TeamType = TEXT("AI");
			TeamName = FString::Printf(TEXT("%s %d"), TeamType, AINum++);
		}
		Row.Add(FText::FromString(TeamName));

		// Column 2: decision state.
		auto Deciding = StaticEnum<EDecisionType>()->
			GetNameByValue(static_cast<int64>(Team->GetDeciding())).ToString();
		auto LastColonIndex = 0;
		if (Deciding.FindLastChar(TEXT(':'), LastColonIndex))
		{
			Deciding = Deciding.Mid(LastColonIndex + 1);
		}
		Row.Add(FText::FromString(Deciding));

		// Column 3:  initiative
		Row.Add(FText::FromString(Team == CurrentTeam ? TEXT("Initiative") : TEXT("")));
		
		TableWidget->SetRow(Row, i + 1);
	}
}
