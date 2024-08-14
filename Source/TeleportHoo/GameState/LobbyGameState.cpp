#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
	
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameState::GetLifetimeReplicatedProps"));
	UE_LOG(LogTemp, Warning, TEXT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));

	DOREPLIFETIME(ALobbyGameState, ConnectedPlayers);
}

bool ALobbyGameState::AreAllPlayersReady() const
{
	for(const FPlayerInfo& PlayerInfo : ConnectedPlayers)
	{
		if(!PlayerInfo.bIsReady)
		{
			return false;
		}
	}
	return true;
}