// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
// #include "LobbyPlayerPlatform.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TELEPORTHOO_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	//FUNCTION
	void SetUpPlatforms();

protected:
	// VARIABLES
	bool IsPlatformsSetUpDone;
};
