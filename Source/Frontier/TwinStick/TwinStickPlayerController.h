 // Copyright 2025 Alex Nye dba Causality Games.

#pragma once

#include "CoreMinimal.h"
#include "Tactical/TacticalPlayerController.h"
#include "TwinStickPlayerController.generated.h"

class AUnit;
class ATelemetry;
class UInputComponent;

UCLASS()
class ATwinStickPlayerController : public ATacticalPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ImpartVelocity(const FVector2D& AnalogInput);
	
private:
	AUnit* GetTargetByRaycast() const;
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Action1();
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Action2();
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Action3();
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Action4();
};