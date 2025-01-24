// Copyright 2025 Alex Nye dba Causality Games.
#include "BaseClasses.h"


void UAction::Start()
{
	State = EActionState::Started;
}


bool UAction::CanResolve() const
{
	const auto Unresolvable = State == EActionState::Ended || State == EActionState::Failed;

	const auto WaitingForTriggerToResolveFirst =
		Trigger &&
		ResolutionTiming == EResolutionTiming::After &&
		Trigger->State != EActionState::Ended;
	
	return !Unresolvable && !WaitingForTriggerToResolveFirst;
}
