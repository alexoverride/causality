// Copyright 2025 Alex Nye dba Causality Games.
#include "TacticalGameMode.h"
#include "TacticalMind.h"
#include "TwinStick/Unit.h"
#include "Tactical/Encounter.h"
#include "Macros.h"
#include "Action/BaseClasses.h"
#include "Kismet/GameplayStatics.h"
#include "Relations/CausalityLattice.h"
#include "Relations/TimeDilator.h"
#include "TwinStick/TwinStickPlayerController.h"
#include "Views/TacticalHUD.h"


ATacticalGameMode::ATacticalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerControllerClass = ATwinStickPlayerController::StaticClass();
	HUDClass = ATacticalHUD::StaticClass();
}


void ATacticalGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Contains all actions taken in the current round in declaration order.
	// Solves for resolution order of action-reaction trees.
	CausalityLattice = NewObject<UCausalityLattice>();
	Time = NewObject<ATimeDilator>();

	const auto Player = Cast<ATacticalPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	const auto AI = GetWorld()->SpawnActor<ATacticalMind>();
	Encounter = GetWorld()->SpawnActor<AEncounter>();
	Encounter->Construct(this, Player, AI);

	HUD = Cast<ATacticalHUD>(Player->GetHUD());

	Teams.Add(Cast<ITacticalTurn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)));
	Teams.Add(Cast<ITacticalTurn>(AI));

	Player->RequestAction();
}


// Any unit can be selected from a team when that team has initiative, if the unit has resources left.
void ATacticalGameMode::Tick(float _)
{
	const auto CurrentTeam = Teams[InitiativeIndex];
	const auto CurrentUnit = CurrentTeam->GetCurrentUnit();
	const auto CurrentTrigger = CausalityLattice->GetNextTrigger();
	
	// Dilate time for each unit proportional to the depth of its most recent reaction in the Causality Lattice.
	Time->DilateBy(CausalityLattice, Teams);


	// End the game?
	auto NumControllersWithUnits = 0;
	for(const auto Team : Teams)
	{
		for(const auto Unit : Team->GetUnits())
		{
			if(Unit->CurrentHP > 0)
			{
				NumControllersWithUnits++;
				break;
			}
		}
	}
	if(NumControllersWithUnits < 2)  return;  // TODO(6):  end the game properly


	
	// Wait for a reaction from this controller?
	for(const auto Team : Teams)
	{
		if(Team->GetDeciding() == EDecisionType::Reaction)
		{
			return;
		}
	}

	
	// A trigger doesn't resolve until all reactions to it are declared or declined.
	if (CurrentTrigger)
	{
		// Get all reactions to the same trigger before getting the reactions to each of those reactions.
		CIRCULATE(InitiativeIndex + 1, Teams,
			// If no one reacts to this trigger anymore, advance to the next.
			if(Teams[i]->RequestReaction())
			{
			 	return;
			}
		);
		CausalityLattice->AdvanceReactionPhase();
	}

	
	if (const auto Action = CausalityLattice->GetNextResolvable())
	{
		Action->Resolve();
		return;
	}

	auto ShouldEmptyLattice = true;
	for (const auto Action : CausalityLattice->GetActions())
	{
		if (Action->State != EActionState::Ended)
		{
			ShouldEmptyLattice = false;
		}
	}
	if (ShouldEmptyLattice)
	{
		CausalityLattice->Empty();
	}

	if (CausalityLattice->GetActions().Num())
	{
		auto AllReactionsUsedOrDeclined = true;
		for(const auto Team : Teams)
		{
			for(const auto Unit : Team->GetUnits())
			{
				const auto ReactionUsed = !Unit->Reaction;
				const auto ReactionDeclined = Unit->TriggersIgnored.Contains(CurrentTrigger);
				if (ReactionUsed || !ReactionDeclined)
				{
					AllReactionsUsedOrDeclined = false;
				}
			}
		}
		if (AllReactionsUsedOrDeclined)
		{
			CausalityLattice->StartResolutionPhase();
			return;
		}
	}
	
	
	// The current controller may have switched Units during reactions.
	// Switch the controller's current Unit back to the Unit whose turn it is.
	for(const auto Unit : CurrentTeam->GetUnits())
	{
		if(CurrentUnit->Actions == 1)
		{
			CurrentTeam->SwitchUnit(Unit);
			CurrentTeam->RequestAction();
		}
	}

	if (CurrentUnit->Actions > 0)
	{
		return;
	}

	// Notify all units that this unit's turn just ended (every unit gets 1 reaction per every unit's turn).
	for(const auto Team : Teams)
	{
		for(const auto Unit : Team->GetUnits())
		{
			Unit->OnTurnEnd();
		}
	}

	
	// Allow other controllers to play a turn if they can, round-robin.
	// Otherwise allow the current controller to go again.
	CIRCULATE(InitiativeIndex + 1, Teams,
		Teams[i]->RequestAction();
		InitiativeIndex = i;

		if(Teams[i]->GetCurrentUnit()->Actions > 0)
		{
			return;
		}
	);

	
	// Start new round
	for(const auto Team : Teams)
	{
		for(const auto Unit : Team->GetUnits())
		{
			Unit->Actions = 2;
		}
	}
	Teams[InitiativeIndex]->RequestAction();
	CausalityLattice->StartActionPhase();
}


void ATacticalGameMode::DeclareAction(UAction* Action) const
{
	CausalityLattice->Add(Action);
	if (!Action->Trigger)
	{
		CausalityLattice->AdvanceReactionPhase();
	}
	Action->Source->Spend(Action);
}
