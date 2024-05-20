// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

// USTRUCT(BlueprintType)

USTRUCT(BlueprintType)
struct FPlayerLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString PlayerName;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTexture2D* AvatarImage;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsReady;
};

UCLASS()
class TELEPORTHOO_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()

public:
	ALobbyGameState();

	virtual void BeginPlay() override;

public:
	// FUNCTION
	UFUNCTION(BlueprintCallable)
	void AddPlayerInfo(const FPlayerLobbyInfo& NewPlayerInfo);
	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(int32 PlayerIndex, bool bReady);
	
public:
	// VARIABLES
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<FPlayerLobbyInfo> PlayersInfo;




	
};
