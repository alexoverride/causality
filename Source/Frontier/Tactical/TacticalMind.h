// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TwinStick/Unit.h"
#include "Tactical/TacticalTurn.h"
#include "TacticalMind.generated.h"

class ATacticalGameMode;

UCLASS()
class ATacticalMind : public AAIController, public ITacticalTurn
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TObjectPtr<AUnit>> Units;
	void SetUp(TArray<AUnit*> NewUnits, ATacticalGameMode* NewGameMode) override;
	const TArray<AUnit*>& GetUnits() const override { return Units; }

	EDecisionType GetDeciding() const override { return Deciding; }
	
	bool RequestReaction() override;
	void RequestAction() override;

	AUnit* GetCurrentUnit() const override { return CurrentUnit; }
	void SwitchUnit(AUnit* NewUnit) override { CurrentUnit = NewUnit; }
	
private:
	void OnPossess(APawn* _) {} // Don't possess, just delegate to the Unit's owned controller.
	void SetBestActionFor(AUnit* Unit, UAction*& InAction, int8& InBestUtility) const;
	static void SetBestReactionFor(const UAction* Trigger, AUnit* Unit, UAction*& InAction, int8& InBestUtility);
	
	UPROPERTY()
	TObjectPtr<AUnit> CurrentUnit = nullptr;
	TObjectPtr<UAction> CurrentAction = nullptr;
	TObjectPtr<ATacticalGameMode> GameMode = nullptr;
	EDecisionType Deciding;
};
