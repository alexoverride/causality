// Copyright 2025 Alex Nye dba Causality Games.
#include "Actions.h"
#include "Relations/Projectiles.h"
#include "TwinStick/Unit.h"


void UShoot::Resolve()
{
	Super::Resolve();

	// TODO(2):  use weapons instead of directly spawning these in the action.
    const auto ForwardVector = Source->GetActorForwardVector();
    const auto Location = Source->GetActorLocation() + ForwardVector * 100.0f + FVector(0, 0, 50);
	auto SpawnParams = FActorSpawnParameters();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    const auto Projectile = Source->GetWorld()->SpawnActor<AProjectile>(Location, FRotator::ZeroRotator, SpawnParams);
	Projectile->Source = Source;
    check(Projectile)
	Cast<UStaticMeshComponent>(Projectile->GetRootComponent())->AddForce(ForwardVector * 20000.f);
}


void UBoost::Resolve()
{
	// TODO(2):  free action,
	// makes the next action before-resolving, at the cost of 50% max HP damage,
	//		The self-damage happens post-effect.
	Super::Resolve();
}
