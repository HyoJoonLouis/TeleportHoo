#include "IngameGameState.h"
#include "../Structs/CharacterStructs.h"
#include "InGameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "IngamePlayerController.h"
#include "Kismet/GameplayStatics.h"

AIngameGameState::AIngameGameState()
{
	RedTeamScore = 0;
	BlueTeamScore = 0;
}


void AIngameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngameGameState, RedTeamScore);
	DOREPLIFETIME(AIngameGameState, BlueTeamScore);
}


void AIngameGameState::AddTeamScore(ETeam WinTeam)
{
	WinTeam == ETeam::RED ? RedTeamScore++ : BlueTeamScore++;
}

void AIngameGameState::OnRep_ScoreChanged()
{
	Cast<AIngamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Client_UpdateScore();
}
