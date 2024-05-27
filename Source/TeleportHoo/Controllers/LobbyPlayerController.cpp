#include "LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

ALobbyPlayerController::ALobbyPlayerController()
{
	
}

void ALobbyPlayerController::UpdatePlayerInfoUI()
{
	//UI 업데이트 로직
	UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController UpdatePlayerInfoUI"));
	UE_LOG(LogTemp, Warning, TEXT("UI 업데이트!!!!"));
}

void ALobbyPlayerController::Client_UpdatePlayerInfo_Implementation()
{
	// 클라이언트에서 플레이어 정보 갱신
	UpdatePlayerInfoUI();
}

void ALobbyPlayerController::ServerSetPlayerInfo_Implementation(const FString& PlayerName, UTexture2D* AvatarImage)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	
	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();
	if(LobbyGameState)
	{
		FPlayerLobbyInfo PlayerLobbyInfo;
		PlayerLobbyInfo.PlayerName = PlayerName;
		PlayerLobbyInfo.AvatarImage = AvatarImage;
		PlayerLobbyInfo.bIsReady = false;

		UE_LOG(LogTemp, Log, TEXT("Player aaaa Name: %s"), *PlayerName);

		
		
		LobbyGameState->AddPlayerInfo(PlayerLobbyInfo);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Player aaaa Sibal"));

	}
}

bool ALobbyPlayerController::ServerSetPlayerInfo_Validate(const FString& PlayerName, UTexture2D* AvatarImage)
{
	return !PlayerName.IsEmpty() && AvatarImage != nullptr;
}