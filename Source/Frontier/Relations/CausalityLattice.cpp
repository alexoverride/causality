// Copyright 2025 Alex Nye dba Causality Games.
#include "CausalityLattice.h"
#include "Action/BaseClasses.h"


// Each of the recursive traversal algorithms assumes:
// - `Actions` is ordered such that reactions come after their triggers.
// - `Actions` is ordered such that all reactions to the same trigger are contiguous (guaranteed by `GetNextTrigger`).


void UCausalityLattice::Add(UAction* Action)
{
	Action->CausalityLattice = this;
	Actions.Add(Action);
}

void UCausalityLattice::Empty()
{
	Actions.Empty();
	CurrentTrigger = -1;
}


uint8 UCausalityLattice::GetReactionDepth(const AUnit* Unit) const
{
	for (auto i = Actions.Num() - 1; i >= 0; i--)
	{
		if (Actions[i]->Source == Unit)
		{
			return GetReactionDepth(Actions[i]);
		}
	}
	return 0;
}


void UCausalityLattice::FailRecursively(const UAction* RootAction)
{
	RootAction->Trigger->State == EActionState::Failed;

	for (const auto Action : Actions)
	{
		if (Action->Trigger->State == EActionState::Failed)
		{
			Action->State = EActionState::Failed;
		}
	}
}


uint8 UCausalityLattice::GetReactionDepth(const UAction* Action)
{
	auto MaybeAction = Action;
	auto i = 0;
	while (MaybeAction)
	{
		i += 1;
		MaybeAction = MaybeAction->Trigger;
	}
	return i;
}


// All reactions to the same trigger must be declared before any reactions are declared to those reactions.
UAction* UCausalityLattice::GetNextTrigger() const
{
	if (Actions.Num() < 1 || CurrentTrigger >= Actions.Num())
	{
		return nullptr;
	}
	return Actions[CurrentTrigger];
}


// The first action within the tree to resolve depends on whether reactions resolve before or after their triggers.
UAction* UCausalityLattice::GetNextResolvable() const
{
	const TFunction<UAction*(UAction*, uint16)>
	GetFirstToResolve = [&] (UAction* Action, uint16 J)
	{
		for(; J < Actions.Num(); J++)
		{
			if(Actions[J]->Trigger == Action && Actions[J]->CanResolve())
			{
				return GetFirstToResolve(Actions[J], J + 1);
			}
		}
		return Action;
	};

	for(auto i = 0; i < Actions.Num(); i++)
	{
		if(Actions[i]->CanResolve())
		{
			return GetFirstToResolve(Actions[i], i + 1);
		}
	}
	return nullptr;
}


void UCausalityLattice::StartActionPhase()
{
	CurrentTrigger = -1;
}

void UCausalityLattice::AdvanceReactionPhase()
{
	CurrentTrigger += 1;
}

void UCausalityLattice::StartResolutionPhase()
{
	CurrentTrigger = 0;
}