// Copyright Alex Nye dba Preposterous Calculations.
#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TacticalCameraManager.generated.h"

// Maintain the pitch of the camera, but its yaw can change.
constexpr float GHeight = 1000.f;
// The camera is always looking down at the target from a 45 degree angle, but can rotate around the target (yaw).
constexpr float GPitch = -45.f;
// How far from the target to follow, maintaining angle and keeping height.  Pythag, sqrt(2).
constexpr float GFollow_Distance = GHeight * 1.41421356237;

UCLASS()
class ATacticalCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AActor* Camera = nullptr;

	virtual void BeginPlay() override;
	virtual void UpdateCamera(float DeltaTime) override;

	void Target(AActor* NewTarget);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	float TranslationTimeElapsed = 1.f;

	AActor* FollowedActor = nullptr;
	FVector PositionAtSwitch;

	static float Ease(float Progress);
};

inline FVector OffsetFromTarget(const float Yaw)
{
	const auto PlanarDistance = GFollow_Distance * cos(FMath::DegreesToRadians(GPitch));
	const auto X = PlanarDistance * cos(FMath::DegreesToRadians(Yaw));
	const auto Y = PlanarDistance * sin(FMath::DegreesToRadians(Yaw));
	return FVector(X, Y, -1 * GHeight);
}
