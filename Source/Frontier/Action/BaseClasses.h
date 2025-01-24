// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "Relations/CausalityLattice.h"
#include "Tactical/Weapons.h"
#include "BaseClasses.generated.h"


class AUnit;
class AWeapon;


enum class EResolutionTiming : uint8
{
	Before, // resolves before its trigger
	After   // resolves after its trigger
};

enum class EActionState : uint8
{
	Pending, Started, Ended, Failed
};

// TODO(2)
enum class EDuration : uint8
{
	Instant,
	Continuous, // Can resolve multiple times, e.g. shooting with a flamethrower over a period of time.  Defined by wep.
};


UCLASS()
class UAction : public UObject
{
	GENERATED_BODY()

public:
	EActionState State = EActionState::Pending;
	EResolutionTiming ResolutionTiming = EResolutionTiming::After;

	bool IsFree = false;

	UPROPERTY()
	TObjectPtr<UAction> Trigger = nullptr;
	
	UPROPERTY()
	TObjectPtr<AUnit> Source = nullptr;
	UPROPERTY()
	TObjectPtr<AUnit> Target = nullptr;
	
	UPROPERTY()
	TObjectPtr<UCausalityLattice> CausalityLattice = nullptr;

	virtual bool CanStart(const UAction* MaybeTrigger) const { return true; }
	virtual void Start();
	virtual bool CanResolve() const;
	virtual void Resolve() { State = EActionState::Ended; }
	virtual void Fail() { CausalityLattice->FailRecursively(this); }
};