// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	
}

void ALobbyPlayerController::ServerSetPlayerInfo_Implementation(const FString& PlayerName, UTexture2D* AvatarImage)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	
	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();
	if(LobbyGameState)
	{
		FPlayerLobbyInfo PlayerLobbyInfo;
		PlayerLobbyInfo.PlayerName = PlayerName;
		PlayerLobbyInfo.AvatarImage = AvatarImage;
		PlayerLobbyInfo.bIsReady = false;

		UE_LOG(LogTemp, Log, TEXT("Player aaaa Name: %s"), *PlayerName);

		
		
		LobbyGameState->AddPlayerInfo(PlayerLobbyInfo);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Player aaaa Sibal"));

	}
}

bool ALobbyPlayerController::ServerSetPlayerInfo_Validate(const FString& PlayerName, UTexture2D* AvatarImage)
{
	return !PlayerName.IsEmpty() && AvatarImage != nullptr;
}