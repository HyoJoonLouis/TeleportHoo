// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, PlayerInfoArray);
}

void ALobbyGameState::AddPlayerInfo(const FPlayerLobbyInfo& NewPlayerInfo)
{
	// PlayersInfo.Add(NewPlayerInfo);
}

void ALobbyGameState::SetPlayerReady(int32 PlayerIndex, bool bReady)
{
	// if(PlayersInfo.IsValidIndex(PlayerIndex))
	// {
	// 	PlayersInfo[PlayerIndex].bIsReady = bReady;
	// }
}
