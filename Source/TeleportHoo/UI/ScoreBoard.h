// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBoard.generated.h"

/**
 * 
 */
UCLASS()
class TELEPORTHOO_API UScoreBoard : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RedScore;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BlueScore;

public:
	virtual void NativeConstruct() override;
	void UpdateScore();
};
