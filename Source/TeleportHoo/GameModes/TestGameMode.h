// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TestGameMode.generated.h"

UCLASS()
class TELEPORTHOO_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
	TSubclassOf<AActor> Platform;
};
