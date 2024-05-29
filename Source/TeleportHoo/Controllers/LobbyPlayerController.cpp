#include "LobbyPlayerController.h"
#include "TeleportHoo/GameState/LobbyGameState.h"
#include "GameFramework/PlayerState.h"
#include "TeleportHoo/UI/LobbyWidget.h"
#include "TeleportHoo/GameModes/LobbyGameMode.h"
#include "TeleportHoo/PlayerState/LobbyPlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "RenderUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemSteam.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "steam/steam_api.h"		// https://partner.steamgames.com/doc/api/steam_api   <- 참고

ALobbyPlayerController::ALobbyPlayerController()
{
	SetActorHiddenInGame(false);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!SteamAPI_Init())
	{
		UE_LOG(LogTemp, Error, TEXT("Steam API 초기화 실패"))
	}

	UE_LOG(LogTemp, Error, TEXT("ALobbyPlayerController::BeginPlay"))
	InitializeLobbyWidget();
}

void ALobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Steam API 종료
	SteamAPI_Shutdown();
	Super::EndPlay(EndPlayReason);
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
	if (LocalPlayerState)
	{
		PlayerName = LocalPlayerState->GetPlayerName();
		if (!PlayerName.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("플레이어 이름 반환 성공"))
			UE_LOG(LogTemp, Log, TEXT("Player Name: %s"), *PlayerName);
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
	if (PlayerName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 이름 못받아와서 Default로 설정"))
		PlayerName = "DefaultPlayerName";
	}


	UE_LOG(LogTemp, Log, TEXT("Player Name: %s"), *PlayerName);
	return PlayerName;
}

UTexture2D* ALobbyPlayerController::GetPlayerAvatar()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub && OnlineSub->GetSubsystemName() == STEAM_SUBSYSTEM)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			APlayerState* LocalPlayerState = GetPlayerState<APlayerState>();
			if (LocalPlayerState)
			{
				// 유니크 플레이어 ID 가져오기
				TSharedPtr<const FUniqueNetId> UserId = Identity->
					GetUniquePlayerId(GetLocalPlayer()->GetControllerId());
				if (UserId.IsValid())
				{
					uint32 AvatarWidth = 0;
					uint32 AvatarHeight = 0;
					if (SteamFriends())
					{
						CSteamID SteamID(*(uint64*)UserId->GetBytes());
						int FriendAvatar = SteamFriends()->GetMediumFriendAvatar(SteamID);
						if (FriendAvatar > 0)
						{
							SteamUtils()->GetImageSize(FriendAvatar, &AvatarWidth, &AvatarHeight);
							uint32 AvatarSize = AvatarWidth * AvatarHeight * 4;
							uint8* AvatarRGBA = new uint8[AvatarSize];
							SteamUtils()->GetImageRGBA(FriendAvatar, AvatarRGBA, AvatarSize);

							IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<
								IImageWrapperModule>(FName("ImageWrapper"));
							TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(
								EImageFormat::PNG);

							if (ImageWrapper.IsValid() && ImageWrapper->SetRaw(
								AvatarRGBA, AvatarSize, AvatarWidth, AvatarHeight, ERGBFormat::BGRA, 8))
							{
								TArray64<uint8> UncompressedRGBA;
								if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedRGBA))
								{
									UTexture2D* AvatarTexture = UTexture2D::CreateTransient(
										AvatarWidth, AvatarHeight, PF_B8G8R8A8);

									// 텍스처 데이터 설정
									void* TextureData = AvatarTexture->GetPlatformData()->Mips[0].BulkData.Lock(
										LOCK_READ_WRITE);
									FMemory::Memcpy(TextureData, UncompressedRGBA.GetData(), UncompressedRGBA.Num());
									AvatarTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
									AvatarTexture->UpdateResource();

									delete[] AvatarRGBA;

									UE_LOG(LogTemp, Warning, TEXT("아바타 반환 성공!!!!!!!!!!!!!!!!!!!!!"));

									return AvatarTexture;
								}
							}

							delete[] AvatarRGBA;
						}
					}
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("아바타 반환 실패........"));
	return nullptr;
}

ULobbyWidget* ALobbyPlayerController::GetLobbyWidgetRef()
{
	if (LobbyWidget)
	{
		return LobbyWidget;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" ALobbyPlayerController::GetLobbyWidgetRef <- LobbyWidget 쓰레기임"));
		return nullptr;
	}
}

void ALobbyPlayerController::Server_ToggleReady_Implementation(bool bIsReady)
{
	if (HasAuthority())
	{
		ALobbyPlayerState* LocalPlayerState = GetPlayerState<ALobbyPlayerState>();
		if (LocalPlayerState)
		{
			LocalPlayerState->PlayerInfo.bIsReady = bIsReady;
			LocalPlayerState->OnRep_PlayerInfo();

			UE_LOG(LogTemp, Error, TEXT("Ready : %s"), bIsReady ? TEXT("true") : TEXT("false"));

			ALobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<ALobbyGameMode>();
			if (GameMode)
			{
				UE_LOG(LogTemp, Warning, TEXT("GameMode->OnPlayerInfoUpdated"));
				GameMode->OnPlayerInfoUpdated();
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LocalPlayerState가 유효하지 않음"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_ToggleReady_Implementation : 권한이 없어!"));
	}
}

void ALobbyPlayerController::InitializeLobbyWidget()
{
	if (!LobbyWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyWidgetClass가 설정되지 않았습니다!"));
		return;
	}

	if (!LobbyWidget)
	{
		this->bShowMouseCursor = true;

		UE_LOG(LogTemp, Warning, TEXT("CreateWidget ULobbyWidget"));
		LobbyWidget = Cast<ULobbyWidget>(CreateWidget(this, LobbyWidgetClass));
		if (IsValid(LobbyWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("LobbyWidget->AddToViewport"));
			LobbyWidget->AddToViewport();

			if (LobbyWidget->IsInViewport())
			{
				UE_LOG(LogTemp, Warning, TEXT("LobbyWidget is successfully added to viewport"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("LobbyWidget failed to add to viewport"));
			}

			LobbyWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

// LobbyUI가 없다면 생성, PlayerLobbyInfo 정보 업데이트
void ALobbyPlayerController::UpdatePlayerInfoUI(int32 PlayerIndex, const FPlayerInfo& PlayerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyPlayerController::UpdatePlayerInfoUI 진입"));

	if (!LobbyWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("LobbyWidget이 설정되지 않았습니다!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("LobbyWidget->UpdatePlayerInfo"));
	LobbyWidget->UpdatePlayerInfo(PlayerIndex, PlayerInfo);
}

void ALobbyPlayerController::ToggleReady()
{
	ALobbyPlayerState* LocalPlayerState = GetPlayerState<ALobbyPlayerState>();
	if (LocalPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("레디 전환"));
		UE_LOG(LogTemp, Warning, TEXT("원래 레디 : %s"),
		       LocalPlayerState->PlayerInfo.bIsReady ? TEXT("true") : TEXT("false"));

		bool bIsReady = !LocalPlayerState->PlayerInfo.bIsReady;
		Server_ToggleReady(bIsReady);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalPlayerState 가 유효하지 않음"));
	}
}
