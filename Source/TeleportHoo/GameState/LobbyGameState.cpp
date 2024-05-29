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