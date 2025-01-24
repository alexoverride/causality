// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "Weapons.generated.h"

class ATargeting;
class AUnit;


UCLASS()
class AWeapon : public AActor
{
	GENERATED_BODY()
public:
	virtual void Fire(AUnit* Target) const;
	virtual TArray<AUnit*> GetTargets(AUnit* Source) const;

private:
	UPROPERTY()
	float Range = 0;
};
