#include "LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "GameFramework/PlayerState.h"
#include "TeleportHoo/UI/LobbyWidget.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "RenderUtils.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemSteam.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

ALobbyPlayerController::ALobbyPlayerController()
{
}

void ALobbyPlayerController::Client_UpdatePlayerInfo_Implementation(int32 PlayerIndex, const FPlayerInfo& PlayerInfo)
{
	// 클라이언트에서 플레이어 정보 갱신
	UpdatePlayerInfoUI(PlayerIndex, PlayerInfo);
}

// 플레이어의 이름 반환
FString ALobbyPlayerController::GetPlayerName()
{
	FString PlayerName;

	// 이미 저장되어있다면 그냥 반환
	APlayerState* LocalPlayerState = GetPlayerState<APlayerState>();
	if(LocalPlayerState)
	{
		PlayerName = LocalPlayerState->GetPlayerName();
		if(!PlayerName.IsEmpty())
		{
			return PlayerName;
		}
	}

	// Subsystem 활용해서 현재 플레이어의 유니크 ID 받아와서 닉네임 가져오기
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			// TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
			TSharedPtr<const FUniqueNetId> UserId = LocalPlayerState->GetUniqueId().GetUniqueNetId();
			if (UserId.IsValid())
			{
				PlayerName = Identity->GetPlayerNickname(*UserId);
			}
		}
	}

	// 위에서 하나도 받아오지 못했다면 기본값 설정
	if(PlayerName.IsEmpty())
	{
		PlayerName = "DefaultPlayerName";
	}

	UE_LOG(LogTemp, Log, TEXT("Player Name: %s"), *PlayerName);
	
	return PlayerName;
}

UTexture2D* ALobbyPlayerController::GetPlayerAvatar()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub && OnlineSub->GetSubsystemName() == STEAM_SUBSYSTEM)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if(Identity.IsValid())
		{
			APlayerState* LocalPlayerState = GetPlayerState<APlayerState>();
			if(LocalPlayerState)
			{
				TSharedPtr<const FUniqueNetId> UserId = LocalPlayerState->GetUniqueId().GetUniqueNetId();
				if(UserId.IsValid())
				{
					uint32 AvatarSize = 0;
					int ImageWidth = 0;
					int ImageHeight = 0;
					uint8* AvatarRGBA = nullptr;

					if(SteamFriends())
					{
						
					}
						
				}
			}
		}
		
	}
}

// LobbyUI가 없다면 생성, PlayerLobbyInfo 정보 업데이트
void ALobbyPlayerController::UpdatePlayerInfoUI(int32 PlayerIndex, const FPlayerInfo& PlayerInfo)
{
	if (!LobbyWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyWidgetClass가 설정되지 않았습니다!"));
		return;
	}
	
	if(!LobbyWidget && LobbyWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateWidget ULobbyWidget"));
		LobbyWidget = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
		if(LobbyWidget)
		{
			UE_LOG(LogTemp, Warning, TEXT("LobbyWidget->AddToViewport"));
			LobbyWidget->AddToViewport();
		}
	}

	if(LobbyWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("LobbyWidget->UpdatePlayerInfo"));
		LobbyWidget->UpdatePlayerInfo(PlayerIndex, PlayerInfo);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyWidget가 쓰레기값"));
	}
}