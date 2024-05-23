#include "IngameGameState.h"
#include "../Structs/CharacterStructs.h"

AIngameGameState::AIngameGameState()
{
	TeamScore.Add(ETeam::NONE, 0);
	TeamScore.Add(ETeam::RED, 0);
	TeamScore.Add(ETeam::BLUE, 0);
}

void AIngameGameState::AddTeamScore(ETeam WinTeam)
{
	TeamScore[WinTeam]++;
}
