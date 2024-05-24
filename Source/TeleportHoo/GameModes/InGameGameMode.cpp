#include "InGameGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/BaseCharacter.h"
#include "../GameModes/IngamePlayerController.h"
#include "../Structs/CharacterStructs.h"
#include "IngameGameState.h"


AInGameGameMode::AInGameGameMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("AInGameGameMode Constructor"));

	bStartPlayersAsSpectators = false;

	CurrentRound = 1;
	MaxRound = 5;
	MaxPlayer = 2;
	isMatchEnd = false;
	TeamScore.Add(ETeam::NONE, 0);
	TeamScore.Add(ETeam::RED, 0);
	TeamScore.Add(ETeam::BLUE, 0);
}

void AInGameGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (ConnectedPlayers.Num() >= MaxPlayer)
		ErrorMessage = "Max Player Exceeded";
}

void AInGameGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AIngamePlayerController* IncomePlayer = Cast<AIngamePlayerController>(NewPlayer);
	
	if (IsValid(IncomePlayer))
	{
		ConnectedPlayers.AddUnique(IncomePlayer);
		IncomePlayer->OnDeadDelegate.AddUniqueDynamic(this, &AInGameGameMode::OnPlayerDiedDelegate);
	}
}

void AInGameGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AIngamePlayerController* ExitingPlayer = Cast<AIngamePlayerController>(Exiting);
	if (ConnectedPlayers.Contains(ExitingPlayer))
	{
		ConnectedPlayers.Remove(ExitingPlayer);
	}
	ReturnToMainMenuHost();
}

bool AInGameGameMode::ReadyToStartMatch_Implementation()
{
	if (ConnectedPlayers.Num() == MaxPlayer)
	{
		GetGameState<AIngameGameState>()->SetTeamScore(ETeam::RED, *TeamScore.Find(ETeam::RED));
		GetGameState<AIngameGameState>()->SetTeamScore(ETeam::BLUE, *TeamScore.Find(ETeam::BLUE));
		for (int i = 0; i < ConnectedPlayers.Num(); i++)
		{
			ABaseCharacter* SpawnedActor = nullptr;

			AActor* PlayerStart = FindPlayerStart(ConnectedPlayers[i], i % MaxPlayer == 1 ? "Red" : "Blue");
			if (i % MaxPlayer == 1)
				ConnectedPlayers[i]->SetControllerTeam(ETeam::RED);
			else if (i % MaxPlayer == 0)
				ConnectedPlayers[i]->SetControllerTeam(ETeam::BLUE);
			SpawnedActor = GetWorld()->SpawnActor<ABaseCharacter>(SpawnActor, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

			ConnectedPlayers[i]->Possess(SpawnedActor);
		}
		return true;
	}
	else
		return false;
}

bool AInGameGameMode::ReadyToEndMatch_Implementation()
{
	return isMatchEnd;
}

void AInGameGameMode::HandleMatchHasEnded()
{
	ReturnToMainMenuHost();
}


void AInGameGameMode::OnPlayerDiedDelegate(AIngamePlayerController* DeadCharacter)
{
	CurrentRound++;

	if (DeadCharacter->GetControllerTeam() == ETeam::RED)
	{
		TeamScore[ETeam::BLUE]++;
		GetGameState<AIngameGameState>()->AddTeamScore(ETeam::BLUE);
	}
	else if (DeadCharacter->GetControllerTeam() == ETeam::BLUE)
	{
		TeamScore[ETeam::RED]++;
		GetGameState<AIngameGameState>()->AddTeamScore(ETeam::RED);
	}

	if (TeamScore[ETeam::RED] >= 3 || TeamScore[ETeam::BLUE] >= 3)
	{
		isMatchEnd = true;
	}
	else
	{
		Cast<AIngamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->Client_UpdateScore();
		SetMatchState(MatchState::WaitingToStart);
		ResetLevel();
	}
}
