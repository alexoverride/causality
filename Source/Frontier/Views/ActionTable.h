#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tactical/TacticalGameMode.h"
#include "ActionTable.generated.h"

class UTableWidget;
class ATacticalTurn;

UCLASS()
class UActionTable : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUp(UWorld* InWorld);
	void Populate() const;

private:
	UPROPERTY()
	TObjectPtr<UTableWidget> TableWidget;
	UPROPERTY()
	TObjectPtr<ATacticalGameMode> GameMode;
};