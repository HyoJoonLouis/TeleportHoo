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

	MaxPlayer = 2;
	isMatchEnd = false;
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
		for (int i = 0; i < ConnectedPlayers.Num(); i++)
		{
			ABaseCharacter* SpawnedActor = nullptr;

			AActor* PlayerStart = FindPlayerStart(ConnectedPlayers[i], i % MaxPlayer == 0 ? "Red" : "Blue");
			if (i % MaxPlayer == 0)
				ConnectedPlayers[i]->SetControllerTeam(ETeam::RED);
			else if (i % MaxPlayer == 1)
				ConnectedPlayers[i]->SetControllerTeam(ETeam::BLUE);
		}
		GetGameState<AIngameGameState>()->OnGameTimeFinished.AddUniqueDynamic(this, &AInGameGameMode::OnGameTimeFinished);
		RoundStart();
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

void AInGameGameMode::RoundStart()
{
	for (const auto& Player : ConnectedPlayers)
	{
		ABaseCharacter* SpawnedActor = nullptr;
		AActor* PlayerStart = FindPlayerStart(Player, Player->GetControllerTeam() == ETeam::RED ? "Red" : "Blue");
		SpawnedActor = GetWorld()->SpawnActor<ABaseCharacter>(SpawnActor, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
		Player->ClientSetRotation(PlayerStart->GetActorRotation());
		Player->Possess(SpawnedActor);
		SpawnedActor->InputBind();
	}
	GetGameState<AIngameGameState>()->AddRound();
	GetGameState<AIngameGameState>()->StartGameTimer();
}

void AInGameGameMode::RoundEnd()
{
	for (const auto& Player : ConnectedPlayers)
	{
		if (Player->GetPawn())
			Player->GetPawn()->Destroy();
		Player->UnPossess();
	}
	RoundStart();
}

void AInGameGameMode::OnGameTimeFinished()
{
	ETeam WinTeam;
	float MaxHealth = 0;
	for (const auto& Player : ConnectedPlayers)
	{
		ABaseCharacter* PlayerPawn = Cast<ABaseCharacter>(Player->GetPawn());
		if (PlayerPawn && PlayerPawn->GetCurrentHealth() > MaxHealth)
		{
			WinTeam = Player->GetControllerTeam();
			MaxHealth = PlayerPawn->GetCurrentHealth();
		}
		PlayerPawn->Server_SetState(ECharacterStates::DEAD);
	}

	AIngameGameState* CurrentGameState = GetGameState<AIngameGameState>();
	CurrentGameState->AddTeamScore(WinTeam);
	if (CurrentGameState->GetTeamScore(ETeam::RED) >= 3 || CurrentGameState->GetTeamScore(ETeam::BLUE) >= 3)
	{
		isMatchEnd = true;
	}
	else
	{
		GetWorldTimerManager().SetTimer(RoundEndTimerHandle, this, &AInGameGameMode::RoundEnd, 3, false);
	}
}


void AInGameGameMode::OnPlayerDiedDelegate(AIngamePlayerController* DeadCharacter)
{
	AIngameGameState* CurrentGameState = GetGameState<AIngameGameState>();
	if (DeadCharacter->GetControllerTeam() == ETeam::RED)
	{
		CurrentGameState->AddTeamScore(ETeam::BLUE);
	}
	else if (DeadCharacter->GetControllerTeam() == ETeam::BLUE)
	{
		CurrentGameState->AddTeamScore(ETeam::RED);
	}

	if (CurrentGameState->GetTeamScore(ETeam::RED) >= 3 || CurrentGameState->GetTeamScore(ETeam::BLUE) >= 3)
	{
		isMatchEnd = true;
	}
	else
	{
		GetWorldTimerManager().SetTimer(RoundEndTimerHandle, this, &AInGameGameMode::RoundEnd, 3, false);
	}
}
