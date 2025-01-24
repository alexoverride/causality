// Copyright 2025 Alex Nye dba Causality Games.
#pragma once

#include "CoreMinimal.h"
#include "BaseClasses.h"
#include "Actions.generated.h"


UCLASS()
class UShoot : public UAction
{
	GENERATED_BODY()

public:
	void Resolve() override;
};


UCLASS()
class UBoost : public UAction
{
	GENERATED_BODY()
public:
	void Resolve() override;
};