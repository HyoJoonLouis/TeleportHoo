#include "IngameGameState.h"
#include "../Structs/CharacterStructs.h"
#include "InGameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "IngamePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AIngameGameState::AIngameGameState()
{
	CurrentRound = 0;
	SecondsAfterGameStart = 0;
	RedTeamScore = 0;
	BlueTeamScore = 0;
}


void AIngameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngameGameState, CurrentRound);
	DOREPLIFETIME(AIngameGameState, SecondsAfterGameStart);
	DOREPLIFETIME(AIngameGameState, RedTeamScore);
	DOREPLIFETIME(AIngameGameState, BlueTeamScore);
}

void AIngameGameState::StartGameTimer()
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, [&]() {
					SecondsAfterGameStart++;
					OnRep_ScoreChanged();
					if (SecondsAfterGameStart >= 180)
					{
						if (OnGameTimeFinished.IsBound())
							OnGameTimeFinished.Broadcast();

						GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
					}
			}, 1.0f, true);
	}
}

void AIngameGameState::ResetGameTimer()
{
	if (HasAuthority())
		SecondsAfterGameStart = 0;
}

void AIngameGameState::StopGameTimer()
{
	if (HasAuthority())
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(GameTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);
		}
	}
}

void AIngameGameState::AddRound()
{
	if (HasAuthority())
	{
		CurrentRound++;
		OnRep_ScoreChanged();
	}
}

void AIngameGameState::AddTeamScore(ETeam WinTeam)
{
	if (HasAuthority())
	{
		WinTeam == ETeam::RED ? RedTeamScore++ : BlueTeamScore++;
		OnRep_ScoreChanged();
	}
}

void AIngameGameState::OnRep_ScoreChanged()
{
	Cast<AIngamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Client_UpdateScore();
}