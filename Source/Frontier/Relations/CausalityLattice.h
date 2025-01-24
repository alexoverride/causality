// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "CausalityLattice.generated.h"

class AUnit;
class UAction;


UCLASS()
class UCausalityLattice : public UObject
{
	GENERATED_BODY()
	
public:
	void Add(UAction* Action);
	void Empty();

	uint8 GetReactionDepth(const AUnit* Unit) const;
	void FailRecursively(const UAction* RootAction);
	
	UAction* GetNextTrigger() const;
	UAction* GetNextResolvable() const;
	void StartActionPhase();
	void AdvanceReactionPhase();
	void StartResolutionPhase();

	const TArray<UAction*>& GetActions() const { return Actions; }

private:
	static uint8 GetReactionDepth(const UAction* Action);

	// The ultimate owner of action objects for GC (once they're declared).
	// Actions are stored in the order they were declared in.
	// Each turn action is a root of a DAG with `Trigger` pointer chains leading to the root.
	UPROPERTY()
	TArray<TObjectPtr<UAction>> Actions;

	int16 CurrentTrigger = -1;
};
