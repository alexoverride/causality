// Copyright 2025 Alex Nye dba Causality Games.
#include "Projectiles.h"


AProjectile::AProjectile()
{
	auto CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	RootComponent = CollisionMesh;

	CollisionMesh->SetSimulatePhysics(true);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	const auto MeshPath = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'");
	const auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(MeshPath);
	check(Mesh.Succeeded());
	CollisionMesh->SetStaticMesh(Mesh.Object);
	CollisionMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* TODO(3)
	const float CollisionRadius = 10.0f; // Set an appropriate collision radius for your projectile
	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
	GetWorld(),
	Start,
	End,
	CollisionRadius,
	UEngineTypes::ConvertToTraceType(ECC_Visibility), // Use Visibility (or appropriate channel)
	false,
	{},
	EDrawDebugTrace::None,
	HitResults,
	true
	);

	if (bHit)
	{
	for (const FHitResult& Hit : HitResults)
	{
	// Handle collision here: Log, destroy the projectile, etc.
	AActor* HitActor = Hit.GetActor();
	if (HitActor && HitActor != this)
	{
	UE_LOG(LogTemp, Warning, TEXT("Projectile hit: %s"), *HitActor->GetName());
	Destroy();
	break;
	}
	}
	}
	*/
}