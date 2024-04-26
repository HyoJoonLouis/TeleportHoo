// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InGameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TELEPORTHOO_API AInGameGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AInGameGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void HandleMatchHasStarted() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> PlayerPawnBPClass;

	UPROPERTY(EditAnywhere)
	uint8 MaxPlayer;
};
