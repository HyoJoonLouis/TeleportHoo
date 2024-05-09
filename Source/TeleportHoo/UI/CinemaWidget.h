// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CinemaWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELEPORTHOO_API UCinemaWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void PlayCinemaAnimation(bool bStart);

protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* StartCinema;

};
