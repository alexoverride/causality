// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitMind.generated.h"


class AUnit;

UCLASS()
class AUnitMind : public AAIController
{
	GENERATED_BODY()
	
public:
	void Tick(const float DeltaTime) override;
	FVector FindBestPosition();
	int8 TestForceAdvantage(const FVector& Location) const;
	int8 TestFollowLeader(const FVector& Vector) const;
	int8 TestDontCluster(const FVector& Vector) const;
	FVector GetNextPathNode(const FVector& GoalLocation) const;
	FVector CalculateDeltaVelocity(
		const FVector& NextPathNode, const FVector& CurrentVelocity
	) const;

	bool bUnderPlayerControl = false;
	FVector BestLocation = FVector::ZeroVector;
	TArray<TObjectPtr<AUnit>>* Friends = nullptr;
	TArray<TObjectPtr<AUnit>>* Enemies = nullptr;
};