#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TeleportHoo/Controllers/LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "TeleportHoo/PlayerState/LobbyPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::StartPlay()
{
	Super::StartPlay();
	// 게임 시작 시 초기화 작업
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	// 게임 시작 시 첫 번째로 호출되는 초기화 작업
}

void ALobbyGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	// 게임 시작 처리
}

void ALobbyGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
	// 게임 대기 상태 처리
}

bool ALobbyGameMode::IsMatchInProgress() const
{
	return Super::IsMatchInProgress();
	// 게임 진행 중인지 확인
}

void ALobbyGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	// 게임 종료 처리
}

void ALobbyGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();
	// 맵 떠날 때 처리
}

void ALobbyGameMode::HandleMatchAborted()
{
	Super::HandleMatchAborted();
	// 게임 중단 처리
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode PostLogin"));

	Super::PostLogin(NewPlayer);
	// 플레이어 로그인 처리

	ALobbyPlayerController* IncomePlayer = Cast<ALobbyPlayerController>(NewPlayer);

	if(IsValid(IncomePlayer))
	{
		ALobbyPlayerState* PlayerState = IncomePlayer->GetPlayerState<ALobbyPlayerState>();
		if(IsValid(PlayerState))
		{	
			PlayerState->PlayerInfo.PlayerName = "PlayerName";
			// PlayerState->PlayerInfo.PlayerName = IncomePlayer->GetPlayerName(); // 플레이어 이름 할당 로직
			PlayerState->PlayerInfo.AvatarImage = nullptr; // 실제 이미지 할당 로직
			PlayerState->PlayerInfo.bIsReady = false;

			ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
			if(IsValid(LobbyGameState))
			{
				LobbyGameState->ConnectedPlayers.AddUnique(PlayerState->PlayerInfo);
				OnPlayerInfoUpdated();
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode Logout"));

	Super::Logout(Exiting);
	// 플레이어 로그아웃 처리

	ALobbyPlayerController* ExitingPlayer = Cast<ALobbyPlayerController>(Exiting);
	if(IsValid(ExitingPlayer))
	{
		ALobbyPlayerState* PlayerState = ExitingPlayer->GetPlayerState<ALobbyPlayerState>();
		if (IsValid(PlayerState))
		{
			ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
			if (IsValid(LobbyGameState))
			{
				LobbyGameState->ConnectedPlayers.Remove(PlayerState->PlayerInfo);
				OnPlayerInfoUpdated();
			}
		}
	}
}

void ALobbyGameMode::OnPlayerInfoUpdated_Implementation()
{
	// 모든 클라이언트에서 동기화 작업 수행
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(*Iterator);
		if (LobbyPlayerController)
		{
			LobbyPlayerController->Client_UpdatePlayerInfo();
		}
	}
}