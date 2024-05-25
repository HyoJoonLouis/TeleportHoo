// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
	bIsLobbyOpen = true;
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, PlayerLobbyInfoArray);
	DOREPLIFETIME(ALobbyGameState, bIsLobbyOpen);
}

// 플레이어 정보 배열에 새롭게 추가
void ALobbyGameState::AddPlayerInfo(const FPlayerLobbyInfo& NewPlayerInfo)
{
	PlayerLobbyInfoArray.Add(NewPlayerInfo);
	OnRep_PlayerLobbyInfoArray();
}

// 특정 플레이어 정보 삭제하기
void ALobbyGameState::RemovePlayerInfo(const FString& PlayerName)
{
	for (int32 i = 0; i < PlayerLobbyInfoArray.Num(); ++i)
	{
		if (PlayerLobbyInfoArray[i].PlayerName == PlayerName)
		{
			PlayerLobbyInfoArray.RemoveAt(i);
			OnRep_PlayerLobbyInfoArray();
			break;
		}
	}
}

// 플레이어 레디 전환
void ALobbyGameState::SetPlayerReady(int32 PlayerIndex, bool bReady)
{
	if (PlayerLobbyInfoArray.IsValidIndex(PlayerIndex))
	{
		// PlayerLobbyInfoArray[PlayerIndex].bIsReady = !PlayerLobbyInfoArray[PlayerIndex].bIsReady;
		PlayerLobbyInfoArray[PlayerIndex].bIsReady = bReady;
		OnRep_PlayerLobbyInfoArray();
	}
}

// 플레이어 정보 가져오기 / 인덱스 -> 구조체
FPlayerLobbyInfo ALobbyGameState::GetPlayerLobbyInfo(int32 PlayerIndex) const
{
	if(PlayerLobbyInfoArray.IsValidIndex(PlayerIndex))
	{
		return PlayerLobbyInfoArray[PlayerIndex];
	}
	return FPlayerLobbyInfo();
}

// 플레이어 인덱스 구하기 / 플레이어명 -> 인덱스
int32 ALobbyGameState::GetPlayerIndexByName(const FString& PlayerName) const
{
	for (int32 i = 0; i < PlayerLobbyInfoArray.Num(); ++i)
	{
		if (PlayerLobbyInfoArray[i].PlayerName == PlayerName)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

// 모든 플레이어가 준비되었나?
bool ALobbyGameState::AreAllPlayersReady() const
{
	for (const FPlayerLobbyInfo& Info : PlayerLobbyInfoArray)
	{
		if (!Info.bIsReady)
		{
			return false;
		}
	}
	return true;
}

// 플레이어 정보 입력 / 인덱스, 이름, 이미지
void ALobbyGameState::UpdatePlayerInfo(int32 PlayerIndex, const FString& NewPlayerName, UTexture2D* NewAvatarImage)
{
	if (PlayerLobbyInfoArray.IsValidIndex(PlayerIndex))
	{
		PlayerLobbyInfoArray[PlayerIndex].PlayerName = NewPlayerName;
		PlayerLobbyInfoArray[PlayerIndex].AvatarImage = NewAvatarImage;
		OnRep_PlayerLobbyInfoArray();
	}
}

const TArray<FPlayerLobbyInfo>& ALobbyGameState::GetAllPlayerInfo() const
{
	return PlayerLobbyInfoArray;
}

void ALobbyGameState::OnRep_PlayerLobbyInfoArray()
{
	UE_LOG(LogTemp, Log, TEXT("PlayerLobbyInfoArray 업데이트 됨!!! OnRep"));
}
