// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "UObject/Interface.h"
#include "TacticalTurn.generated.h"

class ATacticalGameMode;
class UAction;
class AUnit;

UENUM(BlueprintType)
enum class EDecisionType : uint8 { None, Unit, Action, Reaction };


UINTERFACE(MinimalAPI, NotBlueprintable)
class UTacticalTurn : public UInterface
{
	GENERATED_BODY()
};

class ITacticalTurn
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	// Get the current decision type (e.g., selecting a Unit, action, target, etc.)
	virtual EDecisionType GetDeciding() const = 0;

	virtual void SetUp(TArray<AUnit*> NewUnits, ATacticalGameMode* NewGameMode) = 0;
	virtual const TArray<AUnit*>& GetUnits() const = 0;

	// Requests from the game mode to the controller.
	virtual bool RequestReaction() = 0;
	virtual void RequestAction() = 0;

	virtual AUnit* GetCurrentUnit() const = 0;
	virtual void SwitchUnit(AUnit* NewUnit) = 0;
};
