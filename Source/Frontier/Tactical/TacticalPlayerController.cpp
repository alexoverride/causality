// Copyright 2025 Alex Nye dba Causality Games.
#include "TacticalPlayerController.h"
#include "TwinStick/Unit.h"
#include "Action/BaseClasses.h"
#include "Camera/TacticalCameraManager.h"
#include "Tactical/TacticalGameMode.h"
#include "Tactical/TacticalTurn.h"
#include "Tactical/Macros.h"
#include "TwinStick/UnitMind.h"


ATacticalPlayerController::ATacticalPlayerController()
{
	PlayerCameraManagerClass = ATacticalCameraManager::StaticClass();
	Deciding = EDecisionType::Unit;
}


void ATacticalPlayerController::SwitchUnit(AUnit* NewUnit)
{
	const auto Trigger = GameMode->CausalityLattice->GetNextTrigger();
	// Must complete a turn before starting a new one, but reactions are fine.
	if (CurrentUnit->Actions == 1 && !Trigger)
	{
		return;
	}

	Deciding = EDecisionType::Unit;
	Cast<AUnitMind>(CurrentUnit->Controller)->bUnderPlayerControl = false;
	CurrentUnit = NewUnit;
	Cast<ATacticalCameraManager>(PlayerCameraManager)->Target(CurrentUnit);
	Cast<AUnitMind>(CurrentUnit->Controller)->bUnderPlayerControl = true;
}


// We have decided on an action (can be a reaction).
void ATacticalPlayerController::DecideAction(UAction* Action)
{
	const auto MaybeTrigger = GameMode->CausalityLattice->GetNextTrigger();
	
	if (CurrentUnit->TriggersIgnored.Contains(MaybeTrigger))
	{
		return;
	}

	// If it's turn phase, does this unit have Actions?
	if (!CurrentUnit->Has(Action) && !MaybeTrigger)
	{
		return;
	}
		
	Action->Source = CurrentUnit;
	Action->Trigger = MaybeTrigger;

	Deciding = EDecisionType::None;
	GameMode->DeclareAction(Action);

	const auto JustTookLastReaction = MaybeTrigger && !CurrentUnit->Reaction;
	const auto JustTookLastAction = !MaybeTrigger && !CurrentUnit->Actions;
	if (JustTookLastReaction || JustTookLastAction)
	{
		NextUnit();
	}
}


// The game mode requests a reaction.  Check each unit for reactions.
bool ATacticalPlayerController::RequestReaction()
{
	Deciding = EDecisionType::None;
	const auto Trigger = GameMode->CausalityLattice->GetNextTrigger();
	
	for(const auto Unit : Units)
	{
		// Don't bother the player to decide on a reaction they already declined.
		if(Unit->Reaction && !Unit->TriggersIgnored.Contains(Trigger))
		{
			Deciding = EDecisionType::Reaction;
			return true; // Tell the game mode to wait for this unit to decide.
		}
	}

	return false; // Don't wait;  no Units can react.
}


// The game mode requests a turn action.  Continue a turn or choose the first available unit.
void ATacticalPlayerController::RequestAction()
{
	if (CurrentUnit->Actions == 1)
	{
		Deciding = EDecisionType::Action;
	}
	else
	{
		for(const auto Unit : Units)
		{
			if(Unit->Actions == 2)
			{
				SwitchUnit(Unit);
				return;
			}
		}
	}
}

void ATacticalPlayerController::EndTurn()
{
	CurrentUnit->Actions = 0;
	NextUnit();
	Deciding = EDecisionType::None;
}


void ATacticalPlayerController::DeclineReaction()
{
	const auto Trigger = GameMode->CausalityLattice->GetNextTrigger();
	if (!Trigger)
	{
		return;
	}
	CurrentUnit->TriggersIgnored.Add(Trigger);
	Deciding = EDecisionType::None;
	NextUnit();
}


void ATacticalPlayerController::SetUp(const TArray<AUnit*> NewUnits, ATacticalGameMode* NewGameMode)
{
	Units = NewUnits;
	GameMode = NewGameMode;
	CurrentUnit = Units[0];
	for (const auto Unit : Units)
	{
		Unit->SetColor(FLinearColor(0.5f, 0.0f, 0.5f, 1.0f));
	}
	SwitchUnit(Units[0]);
}


void ATacticalPlayerController::NextUnit()
{
	const auto Trigger = GameMode->CausalityLattice->GetNextTrigger();
	// If 1 Action, then the unit has started but not finished a turn.  Disallow switching, except for reactions.
	if(CurrentUnit->Actions == 1 && !Trigger)
	{
		return;
	}

	auto MaybeSwitch = [&](AUnit* Unit) {
		const auto IsReaction = !!Trigger;
		const auto HasReaction = Unit->Reaction && !Unit->TriggersIgnored.Contains(Trigger);
		const auto IsTurn = !IsReaction;
		const auto HasTurn = Unit->Actions > 0;

		if ((IsTurn && HasTurn) || (IsReaction && HasReaction))
		{
			SwitchUnit(Unit);
			return true;
		}
		return false;
	};

	const auto StartIndex = (Units.Find(CurrentUnit) + 1) % Units.Num();
	CIRCULATE(StartIndex, Units,
	{
		if (MaybeSwitch(Units[i]))  return;
	});
}
