#include "LobbyGameMode.h"

#include "ShaderPrintParameters.h"
#include "Kismet/GameplayStatics.h"
#include "TeleportHoo/Controllers/LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "TeleportHoo/PlayerState/LobbyPlayerState.h"

ALobbyGameMode::ALobbyGameMode()
{
}

// 플레이어 로그인 처리
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode PostLogin"));

	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* IncomePlayer = Cast<ALobbyPlayerController>(NewPlayer);

	if (IsValid(IncomePlayer))
	{
		ALobbyPlayerState* PlayerState = IncomePlayer->GetPlayerState<ALobbyPlayerState>();
		if (IsValid(PlayerState))
		{
			PlayerState->PlayerInfo.PlayerName = IncomePlayer->GetPlayerName(); // 플레이어 이름 할당 로직
			PlayerState->PlayerInfo.AvatarImage = nullptr; // 실제 이미지 할당 로직 (구현필요)
			PlayerState->PlayerInfo.bIsReady = false;

			ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
			if (IsValid(LobbyGameState))
			{
				LobbyGameState->ConnectedPlayers.AddUnique(PlayerState->PlayerInfo);
				OnPlayerInfoUpdated();
			}
		}
	}
}

// 로그아웃 처리
void ALobbyGameMode::Logout(AController* Exiting)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode Logout"));

	Super::Logout(Exiting);
	// 플레이어 로그아웃 처리

	ALobbyPlayerController* ExitingPlayer = Cast<ALobbyPlayerController>(Exiting);
	if (IsValid(ExitingPlayer))
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

// 모든 클라이언트에게 최신 플레이어 정보를 동기화
void ALobbyGameMode::OnPlayerInfoUpdated_Implementation()
{
	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (IsValid(LobbyGameState))
	{
		for (int32 i = 0; i < LobbyGameState->ConnectedPlayers.Num(); i++)
		{
			const FPlayerInfo& PlayerInfo = LobbyGameState->ConnectedPlayers[i];

			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++
			     Iterator)
			{
				ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(*Iterator);
				if (LobbyPlayerController)
				{
					LobbyPlayerController->Client_UpdatePlayerInfo(i, PlayerInfo);
				}
			}
		}
	}
}