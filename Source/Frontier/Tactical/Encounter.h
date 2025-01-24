// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "TacticalMind.h"
#include "TacticalPlayerController.h"
#include "Encounter.generated.h"


UCLASS()
class AEncounter : public AActor
{
	GENERATED_BODY()

public:
	void Construct(ATacticalGameMode* GameMode, ATacticalPlayerController* Player, ATacticalMind* AI) const;
	bool bHasEnded = false;
};
