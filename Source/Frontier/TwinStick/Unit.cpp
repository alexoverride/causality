// Copyright 2025 Alex Nye dba Causality Games.
#include "Unit.h"
#include "UnitMind.h"
#include "Action/BaseClasses.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


AUnit::AUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	const auto Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	AIControllerClass = AUnitMind::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	const auto SkeletalMeshPath = TEXT("SkeletalMesh'/Game/SkeletalMeshes/ABasic.ABasic'");
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(SkeletalMeshPath);
	check(SkeletalMesh.Succeeded());
	Mesh->SetSkeletalMesh(SkeletalMesh.Object);

	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(0.f);

	// rotation lock
	const auto PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(Mesh);
	PhysicsConstraint->SetConstrainedComponents(nullptr, NAME_None, Mesh, NAME_None);
	PhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 0.0f);
	PhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 0.0f);
	PhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.0f);
	PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
}


void AUnit::SetColor(FLinearColor Color) const
{
	auto Mesh = Cast<USkeletalMeshComponent>(RootComponent);
	check(Mesh);

	UMaterialInterface* Material = Mesh->GetMaterial(0);
	check(Material);

	UMaterialInstanceDynamic* DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, Material);
	check(DynamicMaterial);
	
	DynamicMaterial->SetVectorParameterValue(FName("Color"), Color);
	check(DynamicMaterial == Mesh->GetMaterial(0));
}


void AUnit::ImpartVelocity(const FVector& NewVelocity) const
{
	const auto Mesh = Cast<USkeletalMeshComponent>(RootComponent);
	const auto Force = NewVelocity * Mesh->GetMass() * MaxDeltaV;
	Mesh->AddForce(Force);
}

void AUnit::OnTurnEnd()
{
        Actions = 2;
        Reaction = 1;
}

void AUnit::Spend(const UAction* Action)
{
        if (Action->IsFree)
        {
                return;
        }
        if (Action->Trigger)
        {
                Reaction -= 1;
        }
        else
        {
                Actions -= 1;
        }
}

bool AUnit::Has(const UAction* Action) const
{
        if (Action->IsFree)
        {
                return true;
        }
        else if (Action->Trigger)
        {
                return Reaction;
        }
        else
        {
                return Actions > 0;
        }
}
