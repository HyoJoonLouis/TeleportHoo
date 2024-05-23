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

	CurrentRound = 1;
	MaxPlayer = 2;
}

void AInGameGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
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

void AInGameGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	AIngamePlayerController* IncomePlayer = Cast<AIngamePlayerController>(NewPlayer);
	ABaseCharacter* SpawnedActor = nullptr;
	if (ConnectedPlayers.Num() % 2 == 1)
	{
		IncomePlayer->SetControllerTeam(ETeam::RED);
		SpawnedActor = GetWorld()->SpawnActor<ABaseCharacter>(SpawnActor, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation());
	}
	else
	{
		IncomePlayer->SetControllerTeam(ETeam::BLUE);
		SpawnedActor = GetWorld()->SpawnActor<ABaseCharacter>(SpawnActor, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation());
	}

	IncomePlayer->Possess(SpawnedActor);
}

void AInGameGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AIngamePlayerController* ExitingPlayer = Cast<AIngamePlayerController>(Exiting);
	if (ConnectedPlayers.Contains(ExitingPlayer))
	{
		ConnectedPlayers.Remove(ExitingPlayer);
	}
}

void AInGameGameMode::OnPlayerDiedDelegate(AIngamePlayerController* DeadCharacter)
{
	if (DeadCharacter->GetControllerTeam() == ETeam::RED)
	{
		GetGameState<AIngameGameState>()->AddTeamScore(ETeam::BLUE);
		if (GetGameState<AIngameGameState>()->GetTeamScore(ETeam::RED) >= MaxRound)
		{

		}
		for (const auto Player : ConnectedPlayers)
		{
			Player->Client_UpdateScore();
		}
	}
	else if (DeadCharacter->GetControllerTeam() == ETeam::BLUE)
	{
		GetGameState<AIngameGameState>()->AddTeamScore(ETeam::RED);
		if (GetGameState<AIngameGameState>()->GetTeamScore(ETeam::BLUE) >= MaxRound)
		{

		}
		for (const auto Player : ConnectedPlayers)
		{
			Player->Client_UpdateScore();
		}
	}
}
