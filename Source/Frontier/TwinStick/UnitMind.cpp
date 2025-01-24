// Copyright 2025 Alex Nye dba Causality Games.
#include "UnitMind.h"

#include "NavigationPath.h"
#include "Unit.h"
#include "NavigationSystem.h"
#include "VectorTypes.h"

// Units move automatically (even on the player's team) unless selected by the player for direct control.
// They do not take actions automatically, however.  Everything in this file is about scoring and performing movement.


void AUnitMind::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Halt automated behavior if under player control or no pawn possessed.
    if (bUnderPlayerControl || !GetPawn())
        return;

    const auto BestPosition = FindBestPosition();
    const auto CurrentVelocity = GetPawn()->GetVelocity();
    const auto NextLocation = GetNextPathNode(BestPosition);
    const auto DeltaVelocity = CalculateDeltaVelocity(NextLocation, CurrentVelocity);
    Cast<AUnit>(GetPawn())->ImpartVelocity(DeltaVelocity);
}

FVector AUnitMind::GetNextPathNode(const FVector& GoalLocation) const
{
    auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    check(NavSys);

    FNavLocation Start;
    NavSys->ProjectPointToNavigation(GetPawn()->GetActorLocation(), Start);
    FNavLocation End;
    NavSys->ProjectPointToNavigation(GoalLocation, End);
    
    auto Path = NavSys->FindPathToLocationSynchronously(GetWorld(), Start, End, GetPawn());
    if (Path && Path->PathPoints.Num() > 1)
    {
        return Path->PathPoints[1]; // Get the first node beyond the current location.
    }
    else
    {
        return GoalLocation;
    }
}


FVector AUnitMind::CalculateDeltaVelocity(const FVector& NextPathNode, const FVector& CurrentVelocity) const
{
    return (NextPathNode - GetPawn()->GetActorLocation()).GetSafeNormal();
}


FVector AUnitMind::FindBestPosition()
{
    auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    check(NavSys);
    
    constexpr auto GridSidePointCount = 50;
    constexpr auto Radius = 10000.f;
    const auto Center = GetPawn()->GetActorLocation();
    
    BestLocation = Center;
    auto BestUtility = 0; 

    for (int32 x = -GridSidePointCount; x <= GridSidePointCount; ++x)
    {
        for (int32 y = -GridSidePointCount; y <= GridSidePointCount; ++y)
        {
            const auto StepLength = Radius / GridSidePointCount;
            const auto RandOffset = FVector(FMath::RandRange(0, 100) * 1.f, FMath::RandRange(0, 100) * 1.f, 0);

            FNavLocation Loc;
            if(NavSys->ProjectPointToNavigation(FVector(x * StepLength, y * StepLength, 0) + Center + RandOffset, Loc))
            {
                auto Utility = 0;
                auto Location = Loc.Location;
            
                if (UE::Geometry::DistanceSquared(Center, Location) > Radius * Radius)
                {
                    continue;
                }
            
                Utility += TestForceAdvantage(Location);
                Utility += TestFollowLeader(Location);
                Utility += TestDontCluster(Location);

                if (Utility > BestUtility) {
                    BestUtility = Utility;
                    BestLocation = Location;
                }
            }
        }
    }

    return BestLocation;
}


int8 AUnitMind::TestForceAdvantage(const FVector& Location) const
{
    auto NumFriends = 0;
    auto NumEnemies = 0;
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetPawn());
    
    ECollisionChannel TraceChannel = ECC_WorldStatic;

    FHitResult HitResult;

    check(Enemies);
    check(Friends);
    
    for (auto Enemy : *Enemies)
    {
        check(Enemy);
        
        const FVector EnemyLocation = Enemy->GetActorLocation();
        const FVector StartLocation = Location;
        const FVector EndLocation = EnemyLocation;
        
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            EndLocation,
            TraceChannel,
            QueryParams
        );
        
        if (!bHit || HitResult.GetActor() == Enemy)
        {
            NumEnemies++;
        }
    }

    for (auto Friend : *Friends)
    {
        check(Friend);
            
        const FVector FriendLocation = Friend->GetActorLocation();
        const FVector StartLocation = Location;
        const FVector EndLocation = FriendLocation;
        
        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            StartLocation,
            EndLocation,
            TraceChannel,
            QueryParams
        );
        
        if (!bHit || HitResult.GetActor() == Friend)
        {
            NumFriends++;
        }
    }

    // Be aggressive on your turn.
    if (Cast<AUnit>(GetPawn())->Actions == 1 && NumEnemies > 0)
    {
        return 1;
    }

    // Otherwise strength in numbers, strength in avoidance.
    return NumFriends - NumEnemies;
}


int8 AUnitMind::TestFollowLeader(const FVector& Vector) const
{
    if (const auto Leader = Friends->FindByPredicate([](const AUnit* Unit)
    {
        return Unit->Actions == 1;
    }))
    {
        return FMath::Min(
            100000.f / (UE::Geometry::DistanceSquared(Vector, (*Leader)->GetActorLocation()) + 1.f),
            3
        );
    }
    return 0;
}


int8 AUnitMind::TestDontCluster(const FVector& Vector) const
{
    for (const auto Friend : *Friends)
    {
        if (100000.f > UE::Geometry::DistanceSquared(Vector, Friend->GetActorLocation()))
        {
            return -3;
        }
    }
    return 0;
}
