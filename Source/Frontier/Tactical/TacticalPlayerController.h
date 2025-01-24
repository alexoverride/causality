// Copyright 2025 Alex Nye dba Causality Games.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Tactical/TacticalTurn.h"
#include "TacticalPlayerController.generated.h"

class UAction;


UCLASS()
class ATacticalPlayerController : public APlayerController, public ITacticalTurn
{
	GENERATED_BODY()

public:
	ATacticalPlayerController();

	void OnPossess(APawn* _) override {} // Never possess;  this controller defers to many characters, each with AI.

	void SetUp(TArray<AUnit*> NewUnits, ATacticalGameMode* NewGameMode) override;
	const TArray<AUnit*>& GetUnits() const override { return Units; }
	void SwitchUnit(AUnit* NewUnit) override;
	UFUNCTION(BlueprintCallable)
	void NextUnit();
	UFUNCTION(BlueprintCallable)
	AUnit* GetCurrentUnit() const override { return CurrentUnit; }

	// Controller state machine
	EDecisionType GetDeciding() const override { return Deciding; }
	UFUNCTION(BlueprintCallable)
	void DecideAction(UAction* Action);
	UFUNCTION(BlueprintCallable)
	void EndTurn();

	// Requests from the game mode's state machine.
	bool RequestReaction() override;
	void RequestAction() override;
	
	UFUNCTION(BlueprintCallable)
	void DeclineReaction();

	// Controllers own Units belonging to a common side, in a 1-to-1 controller-to-team association.
	// See the game mode for notes on how initiative works, but in short the game mode keeps a list of controllers.
	UPROPERTY()
	TArray<TObjectPtr<AUnit>> Units;

	UPROPERTY()
	TObjectPtr<AUnit> CurrentUnit = nullptr;
	EDecisionType Deciding = EDecisionType::Unit;
	TObjectPtr<ATacticalGameMode> GameMode = nullptr;
};
