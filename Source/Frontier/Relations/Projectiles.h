// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "Projectiles.generated.h"

class AUnit;
class UAction;

UCLASS()
class AProjectile : public AActor
{
	GENERATED_BODY()
public:
	AProjectile();
	void Tick(float DeltaTime);

	TObjectPtr<AUnit> Source = nullptr;
};
