#include "LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "../Controllers/LobbyPlayerController.h"
#include "../GameState/LobbyGameState.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, PlayerInfo);
}

// 클라이언트에서 PlayerInfo 변경될때 UI 업데이트
void ALobbyPlayerState::OnRep_PlayerInfo()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
		{
			// PlayerIndex를 구하기 위해서 LobbyGameState를 통해서 확인합니다.
			ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();
			if (LobbyGameState)
			{
				int32 PlayerIndex = LobbyGameState->ConnectedPlayers.IndexOfByKey(PlayerInfo);
				LPC->Client_UpdatePlayerInfo(PlayerIndex, PlayerInfo);
			}
		}
	}
}