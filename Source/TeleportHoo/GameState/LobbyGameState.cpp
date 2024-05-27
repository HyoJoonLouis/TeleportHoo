#include "LobbyGameState.h"

#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
	
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, ConnectedPlayers);
}