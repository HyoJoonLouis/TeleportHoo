// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELEPORTHOO_API URoundResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowResult(int Round, bool isWin);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RoundText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;
};
