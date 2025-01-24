// Copyright 2025 Alex Nye dba Causality Games.

#include "Encounter.h"
#include "TwinStick/Unit.h"
#include "TwinStick/UnitMind.h"

TArray<AUnit*> SpawnGroup(UWorld* World, uint8 NumToSpawn, const FVector& SpawnGridCenter)
{
	TArray<AUnit*> Group;
	for (auto i = 0; i < NumToSpawn; i++)
	{
		// 3x3 spawn grid.
		constexpr float CellSize = 150.f;
		constexpr uint8 GridSize = 3;
		const int32 Row = i / GridSize - GridSize / 2;
		const int32 Col = i % GridSize - GridSize / 2;

		FVector Location(
			Row * CellSize + SpawnGridCenter.X,
			Col * CellSize + SpawnGridCenter.Y,
			0.f + SpawnGridCenter.Z
		);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		auto NewUnit = World->SpawnActor<AUnit>(
			Location,
			FRotator::ZeroRotator,
			SpawnParams
		);
		check(NewUnit);
		
		Group.Add(NewUnit);
	}
	return Group;
}


void AEncounter::Construct(
	ATacticalGameMode* GameMode, ATacticalPlayerController* Player, ATacticalMind* AI
) const
{
	const auto UpHigh = FVector(0.f, 0.f, 300.f);
	const auto OverThere = FVector(300.f, 300.f, 300.f);

	const auto PlayerUnits = SpawnGroup(GetWorld(), 3, UpHigh);
	Player->SetUp(PlayerUnits, GameMode);

	const auto AIUnits = SpawnGroup(GetWorld(), 3, OverThere);
	AI->SetUp(AIUnits, GameMode);

	for (const auto Unit : PlayerUnits)
	{
		Cast<AUnitMind>(Unit->Controller)->Enemies = &(AI->Units);
		Cast<AUnitMind>(Unit->Controller)->Friends = &(Player->Units);
	}
	for (const auto Unit : AIUnits)
	{
		Cast<AUnitMind>(Unit->Controller)->Enemies = &(Player->Units);
		Cast<AUnitMind>(Unit->Controller)->Friends = &(AI->Units);
	}
}
