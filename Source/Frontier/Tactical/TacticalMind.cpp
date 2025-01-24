// Copyright 2025 Alex Nye dba Causality Games.
#include "TacticalMind.h"
#include "Action/Actions.h"
#include "Action/BaseClasses.h"
#include "Tactical/TacticalGameMode.h"
#include "TwinStick/Unit.h"


void ATacticalMind::SetBestActionFor(AUnit* Unit, UAction*& InAction, int8& InBestUtility) const
{
	static const auto ActionClasses = {
		UShoot::StaticClass(),
	};

	const auto BreakTie = [&](int8& SettableUtility, const int8 ComparisonUtility, UAction* Action)
	{
		if (SettableUtility < ComparisonUtility)
		{
			SettableUtility = ComparisonUtility;
			InAction = Action;
		}
		else if (SettableUtility == ComparisonUtility)
		{
			SettableUtility = FMath::RandBool() ? ComparisonUtility : SettableUtility;
		}
	};

	const auto GetUtility = [&](const UAction* Action)
	{
		auto Utility = 0;
		if (Action->IsA(UShoot::StaticClass()))
		{
			auto AttackUtility = 1;
			
			for (const auto Weapon : Unit->Weapons)
			{
				auto WeaponUtility = 0;
				for (const auto Target : Weapon->GetTargets(Action->Source))
				{
					// Penalize friendly fire, reward multiple targeting.
					if (Units.Contains(Target))
					{
						WeaponUtility -= 1;
					}
					else
					{
						WeaponUtility += 1;
					}
				}
				if (WeaponUtility > AttackUtility)
				{
					AttackUtility = WeaponUtility;
				}
			}
			if (AttackUtility > Utility)
			{
				Utility = AttackUtility;
			}
		}

		return Utility;
	};

	// Continue an action, such as holding a flamethrower valve open.
	if (CurrentAction && CurrentAction->Source == Unit && CurrentAction->CanResolve())
	{
		GetUtility(CurrentAction);
	}

	for (const auto ActionClass : ActionClasses)
	{
		const auto ClassName = ActionClass->GetName();
		const auto UniqueName = MakeUniqueObjectName(GetTransientPackage(), ActionClass, *ClassName);
		const auto Action = NewObject<UAction>(GetTransientPackage(), ActionClass, UniqueName);
		
		Action->Source = Unit;
		if (Unit->Has(Action))
		{
			BreakTie(InBestUtility, GetUtility(Action), Action);
		}
	}
}


void ATacticalMind::SetBestReactionFor(const UAction* Trigger, AUnit* Unit, UAction*& InAction, int8& InBestUtility)
{
	static const auto ActionClasses = {
		UShoot::StaticClass(),
		// TODO(6):  Boosted reactions
	};

	const auto BreakTie = [&](int8& SettableUtility, const int8 ComparisonUtility, UAction* Action)
	{
		if (SettableUtility < ComparisonUtility)
		{
			SettableUtility = ComparisonUtility;
			InAction = Action;
		}
		else if (SettableUtility == ComparisonUtility)
		{
			SettableUtility = FMath::RandBool() ? ComparisonUtility : SettableUtility;
		}
	};
	
	const auto Utility = [&](const UAction* Action) -> int8
	{
		if (Trigger->IsA(UShoot::StaticClass()) && Action->IsA(UShoot::StaticClass()))
		{
			// TODO(2):  if the reaction shot would hit before the trigger shot (dist*dilation <), return 2.
			return 1;
		}

		// TODO(6):  other actions and triggers
		return 0;
	};

	for (const auto ActionClass : ActionClasses)
	{
		const auto Action = NewObject<UAction>(ActionClass);
		Action->Source = Unit;
		if (Unit->Has(Action))
		{
			BreakTie(InBestUtility, Utility(Action), Action);
		}
	}
}


// Request from the game mode for the controller to choose a reaction.
bool ATacticalMind::RequestReaction()
{
	const auto Trigger = GameMode->CausalityLattice->GetNextTrigger();
	UAction* BestReaction = nullptr;
	int8 BestUtility = 0;

	for(const auto Unit : Units)
	{
		if (Unit->Actions != 0)
		{
			SetBestReactionFor(Trigger, Unit, BestReaction, BestUtility);
		}
	}

	// Do not react if our best reaction has negative utility.
	if(BestReaction)
	{
		BestReaction->Trigger = Trigger;
		GameMode->DeclareAction(BestReaction);
		CurrentAction = BestReaction;
		Deciding = EDecisionType::None;
	}

	return !!BestReaction;
}


// Request from the game mode for the controller to choose a turn action.
// Pick the best available action from all active units, or none if all have negative utility.
void ATacticalMind::RequestAction()
{
	UAction* BestAction = nullptr;
	int8 BestUtility = -1;

	auto BestActionFor = [&](AUnit* Unit)
	{
		if(Unit->Actions)
		{
			SetBestActionFor(Unit, BestAction, BestUtility);
		}
	};

	if(CurrentUnit->Actions)
	{
		BestActionFor(CurrentUnit);
	}
	else for (const auto Unit : Units)
	{
		BestActionFor(Unit);
	}
	
	if(BestAction)
	{
		if (CurrentUnit != BestAction->Source)
		{
			SwitchUnit(BestAction->Source);
		}
		CurrentAction = BestAction;
		GameMode->DeclareAction(BestAction);
		Deciding = EDecisionType::None;
		if (!CurrentUnit->Actions)
		{
			for (const auto Unit : Units)
			{
				if (Unit->Actions)
				{
					SwitchUnit(Unit);
					return;
				}
			}
		}
	}
	else
	{
		Deciding = EDecisionType::Action;
	}
}


void ATacticalMind::SetUp(TArray<AUnit*> NewUnits, ATacticalGameMode* NewGameMode)
{
	Units = NewUnits;
	GameMode = NewGameMode;
	check(NewUnits.Num());
	for (auto Unit : Units)
	{
		Unit->SetColor(FLinearColor(0.5f, 1.0f, 0.5f, 1.0f));
	}
	CurrentUnit = Units[0];
}
