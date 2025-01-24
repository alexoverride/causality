// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "TimeDilator.generated.h"


class ITacticalTurn;
class UCausalityLattice;

UCLASS()
class ATimeDilator : public AActor
{
	GENERATED_BODY()
public:
	static void DilateBy(
		const UCausalityLattice* CausalityLattice,
		TArray<ITacticalTurn*> Teams
	);
};
