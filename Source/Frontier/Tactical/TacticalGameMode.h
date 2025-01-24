// Copyright 2025 Alex Nye dba Causality Games.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "TacticalGameMode.generated.h"

class ATacticalHUD;
class UCausalityLattice;
class AEncounter;
class ATimeDilator;
class UAction;
class ITacticalTurn;


UCLASS()
class ATacticalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATacticalGameMode();
	virtual void BeginPlay() override;
	void Tick(float _) override;

	void DeclareAction(UAction* Action) const;

	TArray<ITacticalTurn*> Teams;
	int8 InitiativeIndex = 0;

	UPROPERTY()
	TObjectPtr<UCausalityLattice> CausalityLattice = nullptr;
	TObjectPtr<ATacticalHUD> HUD;

private:
	UPROPERTY()
	TObjectPtr<ATimeDilator> Time = nullptr;
	UPROPERTY()
	TObjectPtr<AEncounter> Encounter = nullptr;
};