#pragma once

#include "Table.h"
#include "TeamsTable.generated.h"

class ATacticalGameMode;


UCLASS()
class UTeamsTable : public UUserWidget
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