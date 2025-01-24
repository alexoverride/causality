// Copyright 2025 Alex Nye dba Causality Games.
#include "TimeDilator.h"
#include "CausalityLattice.h"
#include "Projectiles.h"
#include "Tactical/TacticalTurn.h"
#include "TwinStick/Unit.h"


void ATimeDilator::DilateBy(
	const UCausalityLattice* CausalityLattice,
	TArray<ITacticalTurn*> Teams
) {
	for (const auto Team : Teams)
	{
		for (const auto Unit : Team->GetUnits())
		{
			const auto ReactionDepth = CausalityLattice->GetReactionDepth(Unit);
			Unit->CustomTimeDilation /= FMath::Loge(ReactionDepth + 3.f);
			
			// TODO(2):  when firing, inherit CTD from unit.  Also no grav on projectiles.
			/*
			for (const auto Projectile : Projectiles)
			{
				if (Projectile->Source == Unit)
				{
					Projectile->CustomTimeDilation = 1 / DilationTransition;
				}
			}
			*/
		}
	}
}
