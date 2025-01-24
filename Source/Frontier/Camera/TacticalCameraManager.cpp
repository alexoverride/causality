// Copyright 2025 Alex Nye dba Causality Games.
#include "TacticalCameraManager.h"

#include "TacticalCamera.h"
#include "GameFramework/Actor.h"


void ATacticalCameraManager::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	PrimaryActorTick.bCanEverTick = true;
	Camera = GetWorld()->SpawnActor<ATacticalCamera>(ATacticalCamera::StaticClass());
}

void ATacticalCameraManager::BeginPlay() {  SetViewTarget(Camera);  }


// Core requirements:
//  - follows the TargetActor at FOLLOW_DISTANCE and HEIGHT,
//  - moves toward the next target when the FollowedActor changes,
//  - automatically rotates with the target,
//  - and eases through all of these transitions.
void ATacticalCameraManager::UpdateCamera(const float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);
	TranslationTimeElapsed += DeltaTime;

	if(!FollowedActor) return;

	const auto Easing = Ease(TranslationTimeElapsed);
	const auto Yaw = GetOwningPlayerController()->GetControlRotation().Yaw;
	const auto Rotation = FRotator(GPitch, Yaw, 0.f);
	
	// Calculate the position the camera needs to be in to point at the followed actor given its current rotation.
	const auto PolarOffset = OffsetFromTarget(Yaw);
	const auto Destination = FollowedActor->GetActorLocation() - PolarOffset;

	const auto LerpedDestinations = FMath::Lerp(
		PositionAtSwitch,
		Destination,
		Easing
	);

	Camera->SetActorLocationAndRotation(LerpedDestinations, Rotation);
}


// Tells the camera manager our goal target, doesn't immediately go to.
// Unless it's the first target, then it goes immediately.
void ATacticalCameraManager::Target(AActor* NewTarget)
{
	// Basically just here to prevent strange camera movement when switching from a unit to itself.
	if(NewTarget == FollowedActor) return;
	if(!FollowedActor)
	{
		Camera->SetActorLocationAndRotation(
			NewTarget->GetActorLocation() - OffsetFromTarget(NewTarget->GetActorRotation().Yaw),
			FRotator(GPitch, NewTarget->GetActorRotation().Yaw, 0.f)
		);
	}
	
	PositionAtSwitch = Camera->GetActorLocation();

	TranslationTimeElapsed = 0.f;
	FollowedActor = NewTarget;
}


// Ease in out cubic.  Movement is slow near zero and 1, but fast near 0.5.
// Produces 1 second constant travel time to a stationary target.
float ATacticalCameraManager::Ease(float Progress)
{
	Progress = FMath::Clamp(Progress, 0.0f, 1.0f);
	return Progress < 0.5 ? 4 * Progress * Progress * Progress : 1 - pow(-2 * Progress + 2, 3) / 2;
}
