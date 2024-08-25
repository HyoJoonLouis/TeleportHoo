#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
	
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameState::GetLifetimeReplicatedProps"));

	DOREPLIFETIME(ALobbyGameState, ConnectedPlayers);
}

bool ALobbyGameState::AreAllPlayersReady() const
{
	// 최소 두 명 이상의 플레이어가 연결되어 있어야 시작할 수 있음
	if (ConnectedPlayers.Num() < 2)
	{
		return false;
	}

	// 모든 플레이어가 준비되었는지 확인
	for (const FPlayerInfo& PlayerInfo : ConnectedPlayers)
	{
		if (!PlayerInfo.bIsReady)
		{
			return false;
		}
	}

	return true;
}