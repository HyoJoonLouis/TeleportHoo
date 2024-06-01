#include "LobbyGameMode.h"
#include "ShaderPrintParameters.h"
#include "Kismet/GameplayStatics.h"
#include "TeleportHoo/Controllers/LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "TeleportHoo/PlayerState/LobbyPlayerState.h"
#include "HooGameInstance.h"

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
			PlayerState->PlayerInfo.AvatarImage = IncomePlayer->GetPlayerAvatar(); // 실제 이미지 할당 로직
			PlayerState->PlayerInfo.bIsReady = false;

			ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
			if (IsValid(LobbyGameState))
			{
				LobbyGameState->ConnectedPlayers.AddUnique(PlayerState->PlayerInfo);

				UE_LOG(LogTemp, Error, TEXT("Player1 Name : %s"), *LobbyGameState->ConnectedPlayers[0].PlayerName);
				UE_LOG(LogTemp, Error, TEXT("Player1 Ready : %s"),
					   LobbyGameState->ConnectedPlayers[0].bIsReady ? TEXT("true") : TEXT("false"));

				if (LobbyGameState->ConnectedPlayers.Num() == 2)
				{
					UE_LOG(LogTemp, Error, TEXT("Player2 Name: %s"), *LobbyGameState->ConnectedPlayers[1].PlayerName);
					UE_LOG(LogTemp, Error, TEXT("Player2 Ready : %s"),
						   LobbyGameState->ConnectedPlayers[1].bIsReady ? TEXT("true") : TEXT("false"));
				}

				OnPlayerInfoUpdated();

				// 호스트에게만 시작 버튼을 활성화
				if (IncomePlayer->HasAuthority())
				{
					UE_LOG(LogTemp, Error, TEXT("시작버튼 활성화"));
					IncomePlayer->Client_SetStartButtonVisibility(true);
				}

				// 새로운 플레이어에게 방 이름 설정
				UHooGameInstance* GameInstance = Cast<UHooGameInstance>(GetGameInstance());
				if (GameInstance)
				{
					UE_LOG(LogTemp, Error, TEXT("방제목 설정"));
					FString ServerName = GameInstance->GetCreateServerName();
					UE_LOG(LogTemp, Warning, TEXT("방제목 : %s"), *ServerName);
					IncomePlayer->Client_SetServerName(ServerName);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LobbyGameState is nullptr"));
				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerState is nullptr"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IncomePlayer is nullptr"));
		return;
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

void ALobbyGameMode::CheckAllPlayersReady()
{
	UE_LOG(LogTemp, Error, TEXT("ALobbyGameMode::CheckAllPlayersReady 진입"));

	ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
	if (IsValid(LobbyGameState) && LobbyGameState->AreAllPlayersReady())
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(*Iterator);
			if (IsValid(LobbyPlayerController))
			{
				if (LobbyPlayerController->HasAuthority())
				{
					LobbyPlayerController->Client_SetStartButtonVisibility(true);
				}
			}
		}
	}
	else
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(*Iterator);
			if (IsValid(LobbyPlayerController))
			{
				if (LobbyPlayerController->HasAuthority())
				{
					LobbyPlayerController->Client_SetStartButtonVisibility(false);
				}
			}
		}
	}
}

// 모든 클라이언트에게 최신 플레이어 정보를 동기화
void ALobbyGameMode::OnPlayerInfoUpdated_Implementation()
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("ALobbyGameMode : OnPlayerInfoUpdated_Implementation 진입"));

        ALobbyGameState* LobbyGameState = GetGameState<ALobbyGameState>();
        if (!IsValid(LobbyGameState))
        {
            UE_LOG(LogTemp, Error, TEXT("LobbyGameState가 유효하지 않음"));
            return;
        }

        UE_LOG(LogTemp, Warning, TEXT("LobbyGameState->ConnectedPlayers.Num : %d"), LobbyGameState->ConnectedPlayers.Num());

        bool bAllPlayersReady = true;

        for (int32 i = 0; i < LobbyGameState->ConnectedPlayers.Num(); i++)
        {
            if (!LobbyGameState->ConnectedPlayers.IsValidIndex(i))
            {
                UE_LOG(LogTemp, Error, TEXT("ConnectedPlayers 인덱스가 유효하지 않음: %d"), i);
                continue;
            }

            const FPlayerInfo& PlayerInfo = LobbyGameState->ConnectedPlayers[i];
            if (!PlayerInfo.bIsReady)
            {
                bAllPlayersReady = false;
            }

            for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
            {
                ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(*Iterator);
                if (IsValid(LobbyPlayerController))
                {
                    UE_LOG(LogTemp, Warning, TEXT("LobbyPlayerController->Client_UpdatePlayerInfo"));
                    UE_LOG(LogTemp, Warning, TEXT("PlayerIndex: %d, PlayerName: %s, ReadyStatus: %s"), i, *PlayerInfo.PlayerName, PlayerInfo.bIsReady ? TEXT("READY") : TEXT("NOT READY"));
                    LobbyPlayerController->Client_UpdatePlayerInfo(i, PlayerInfo);
					LobbyPlayerController->Client_SetStartButtonVisibility(false);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("PlayerController가 유효하지 않음"));
                }
            }
        }

        // 모든 클라이언트에게 Start 버튼의 활성화 상태를 업데이트
        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(*Iterator);
            if (IsValid(LobbyPlayerController))
            {
				if(LobbyPlayerController->HasAuthority())
				{
					LobbyPlayerController->Client_SetStartButtonEnabled(bAllPlayersReady);
					LobbyPlayerController->Client_SetStartButtonVisibility(true);
				}
				else
				{
					LobbyPlayerController->Client_SetStartButtonVisibility(false);
				}
			}
		}
	}
}