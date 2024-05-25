// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

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

// FUNCTION
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable)
	void AddPlayerInfo(const FPlayerLobbyInfo& NewPlayerInfo);
	UFUNCTION(BlueprintCallable)
	void RemovePlayerInfo(const FString& PlayerName);
	UFUNCTION(BlueprintCallable)
	void SetPlayerReady(int32 PlayerIndex, bool bReady);
	UFUNCTION(BlueprintCallable)
	FPlayerLobbyInfo GetPlayerLobbyInfo(int32 PlayerIndex) const;
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerIndexByName(const FString& PlayerName) const;
	UFUNCTION(BlueprintCallable)
	bool AreAllPlayersReady() const;
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerInfo(int32 PlayerIndex, const FString& NewPlayerName, UTexture2D* NewAvatarImage);


	UFUNCTION(BlueprintPure)
	const TArray<FPlayerLobbyInfo>& GetAllPlayerInfo() const;
	
	UFUNCTION()
	void OnRep_PlayerLobbyInfoArray();
	
// VARIABLES
public:
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "GameState")
	TArray<FPlayerLobbyInfo> PlayerLobbyInfoArray;
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "GameState")
	bool bIsLobbyOpen;
};