#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Table.generated.h"

class UTableWidget;
class UOverlay;
class UImage;
class UTextBlock;
class UHorizontalBox;
class UVerticalBox;


UCLASS()
class URowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetUp(const int8 RowIndex);
	void SetNumColumns(const int8 NumColumns);
	void SetColumns(const TArray<FText>& NewColumns) const;

private:
	UPROPERTY()
	TObjectPtr<UHorizontalBox> Container;
	UPROPERTY()
	TObjectPtr<UOverlay> Background;
	UPROPERTY()
	TArray<TObjectPtr<UTextBlock>> Columns;
	UPROPERTY()
	TObjectPtr<UImage> Image;
};


UCLASS()
class UTableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUp();
	void SetDimensions(const int8 NumRows, const int8 NumColumns);
	void SetTitle(const FText& NewTitle) const;
	void SetRow(const TArray<FText>& NewColumns, const int8 RowIndex);

private:
	UPROPERTY()
	TObjectPtr<UVerticalBox> Container;
	UPROPERTY()
	TObjectPtr<UTextBlock> Title;
	UPROPERTY()
	TArray<TObjectPtr<URowWidget>> Rows;
};