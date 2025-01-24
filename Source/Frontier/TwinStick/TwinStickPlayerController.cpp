// Copyright 2025 Alex Nye dba Causality Games.


#include "TwinStickPlayerController.h"

#include "Action/Actions.h"
#include "TwinStick/Unit.h"


void ATwinStickPlayerController::ImpartVelocity(const FVector2D& AnalogInput)
{
    const auto X = AnalogInput.X;
    const auto Y = AnalogInput.Y;
    constexpr auto Z = 0.f;
    const auto Vec = FVector(X, Y, Z);
    if (CurrentUnit)
    {
        // Get the rotation to apply to the inputs, so inputs are relative to camera.
        const auto CurrentRotation = GetControlRotation();
        const auto Rad = FMath::DegreesToRadians(CurrentRotation.Yaw);
        const auto Cos = FMath::Cos(Rad);
        const auto Sin = FMath::Sin(Rad);
        const auto Mat = FMatrix(FVector(Cos, Sin, 0), FVector(-Sin, Cos, 0), FVector(0, 0, 1), FVector(0, 0, 0));
        const auto RotatedVel = Mat.TransformVector(Vec);
            
        CurrentUnit->ImpartVelocity(RotatedVel);
    }
}


// TODO(2):  belongs in weapon.  Pass the control rotation all the way to proj spawn.
AUnit* ATwinStickPlayerController::GetTargetByRaycast() const
{
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetPawn());

    constexpr auto TraceChannel = ECC_Visibility;
    FHitResult HitResult;
    
    const auto StartLocation = CurrentUnit->GetActorLocation();
    const auto EndLocation = GetControlRotation().Vector() * 1000.f;

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartLocation,
        EndLocation,
        TraceChannel,
        QueryParams
    );

    if (bHit)  return Cast<AUnit>(HitResult.GetActor());
    return nullptr;
}

void ATwinStickPlayerController::Action1()
{
    const auto Action = NewObject<UShoot>(this);
    Action->Source = CurrentUnit;
    Action->Target = GetTargetByRaycast();
    Action->Start();
    DecideAction(Action);
}

void ATwinStickPlayerController::Action2()
{
    // TODO(2)
}

void ATwinStickPlayerController::Action3()
{
    // TODO(2)
}

void ATwinStickPlayerController::Action4()
{
    // TODO(2)
}