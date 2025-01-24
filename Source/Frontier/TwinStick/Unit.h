// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "Unit.generated.h"

class UPhysicsConstraintComponent;
class AWeapon;
class UAction;


UCLASS()
class AUnit : public APawn
{
	GENERATED_BODY()

public:
	AUnit();
	void SetColor(FLinearColor Color) const;

	int8 Actions = 2;
	bool Reaction = 1;

	uint8 MaxHP = 100;
	const float MaxDeltaV = 1000.f;

	void ImpartVelocity(const FVector& NewVelocity) const;
	void OnTurnEnd();
	void Spend(const UAction* Action);
	bool Has(const UAction* Action) const;
	TSet<UAction*> TriggersIgnored;
	
	UPROPERTY(SaveGame)
	uint8 CurrentHP = 100;
	UPROPERTY(SaveGame)
	TArray<TObjectPtr<AWeapon>> Weapons;
	UPROPERTY(SaveGame)
	TObjectPtr<AWeapon> CurrentWeapon = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint = nullptr;
};
