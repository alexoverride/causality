// Copyright Alex Nye dba Preposterous Calculations.
#include "TacticalCamera.h"
#include "Camera/CameraComponent.h"

ATacticalCamera::ATacticalCamera()
{
	RootComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
}