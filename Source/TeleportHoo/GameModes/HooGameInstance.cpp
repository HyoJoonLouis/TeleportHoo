// Fill out your copyright notice in the Description page of Project Settings.

#include "HooGameInstance.h"
#include "Engine/World.h"
#include "engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>
#include "interfaces/OnlineIdentityInterface.h"
#include "UObject/ConstructorHelpers.h"

UHooGameInstance::UHooGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("UHooGameInstance Constructor"));

	MySessionName = FName("My Session");

	InitializeMaps();

	SelectedMapName = "SnowCastle";
	SelectedMapURL = "/Game/Levels/L_SnowCastle";
}

void UHooGameInstance::Init()
{
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind Delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UHooGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UHooGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UHooGameInstance::OnJoinSessionComplete);
		}
	}
}

void UHooGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	if (bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/Levels/L_Lobby?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed : OnCreateSessionComplete"));
	}
}

void UHooGameInstance::OnFindSessionsComplete(bool bSucceeded)
{
	SearchingForServerDel.Broadcast(false);

	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded : %d"), bSucceeded);

	if (bSucceeded)
	{
		int32 ArrayIndex = -1;

		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			++ArrayIndex;

			if (!Result.IsValid())
				continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString ServerMapName = "Empty Server MapName";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_MAPNAME_KEY"), ServerMapName);

			Info.ServerName = ServerName;
			Info.ServerMapName = ServerMapName;
			UE_LOG(LogTemp, Warning, TEXT("ServerMapName : %s"), *Info.ServerMapName);

			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.SetPlayerCount();
			Info.SelectedMapName = SelectedMapName;

			Info.Ping = Result.PingInMs;
			Info.ServerArrayIndex = ArrayIndex;

			ServerListDel.Broadcast(Info);
			UE_LOG(LogTemp, Warning, TEXT("Ping : %d"), Info.Ping);
		}

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count : %d"), SessionSearch->SearchResults.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed : OnFindSessionsComplete"));
	}
}

void UHooGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : JoinAddress is \"\""));
		}
	}
}

void UHooGameInstance::CreateServer(FCreateServerInfo ServerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;

	// 스팀, 엔진 테스트 전환하려면 DefaultEngine.ini에서 DefaultPlatformService 를 Steam, NULL 을 전환해주면 됨.
	// Set to use server Info for Lan in future
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	{
		SessionSettings.bIsLANMatch = false;
		// Steam으로 서버 만드려면, 이 옵션이 필수적으로 들어가야함
		SessionSettings.bUseLobbiesIfAvailable = true;
		UE_LOG(LogTemp, Warning, TEXT("CreateServer -> IsSteam"));
	}
	else
	{
		SessionSettings.bIsLANMatch = true; // Is LAN
		SessionSettings.bUseLobbiesIfAvailable = false;
		UE_LOG(LogTemp, Warning, TEXT("CreateServer -> IsLan"));
	}

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = ServerInfo.MaxPlayers;
	SessionSettings.Set(L"SERVER_NAME_KEY", ServerInfo.ServerName,
	                    EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(L"SERVER_MAPNAME_KEY", ServerInfo.ServerMapName,
	                    EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UHooGameInstance::FindServer()
{
	SearchingForServerDel.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("FindServer"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	{
		SessionSearch->bIsLanQuery = false;
		UE_LOG(LogTemp, Warning, TEXT("FindServer -> IsSteam"));
	}
	else
	{
		SessionSearch->bIsLanQuery = true; // Is LAN
		UE_LOG(LogTemp, Warning, TEXT("FindServer -> IsLan"));
	}
	SessionSearch->MaxSearchResults = 1000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UHooGameInstance::JoinServer(int32 ArrayIndex)
{
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
	if (Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("JOINING SERVER AT INDEX : %d"), ArrayIndex);
		SessionInterface->JoinSession(0, MySessionName, Result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO JOIN SERVER AT INDEX : %d"), ArrayIndex);
	}
}

void UHooGameInstance::FillMapList()
{
	MapList.Empty();
	InitializeMaps();

	for (FMapInfo Map : MapList)
		FMapNameDel.Broadcast(Map.MapName);
}

UTexture2D* UHooGameInstance::GetMapImage(FString MapName)
{
	for (FMapInfo Map : MapList)
	{
		if (Map.MapName.Equals(MapName))
			return Map.MapImage;
	}
	return nullptr;
}

UTexture2D* UHooGameInstance::GetMapOverviewImage(FString MapName)
{
	for (FMapInfo Map : MapList)
	{
		if (Map.MapName.Equals(MapName))
			return Map.MapOverviewImage;
	}
	return nullptr;
}

void UHooGameInstance::SetSelectedMap(FString MapName)
{
	for (FMapInfo Map : MapList)
		if (Map.MapName.Equals(MapName))
		{
			SelectedMapName = Map.MapName;
			UE_LOG(LogTemp, Warning, TEXT("SelectedMapName : %s"), *SelectedMapName);

			SelectedMapURL = Map.MapURL;
			UE_LOG(LogTemp, Warning, TEXT("SelectedMapURL : %s"), *SelectedMapURL);
		}
}

FString UHooGameInstance::GetSelectedMapName()
{
	return SelectedMapName;
}

void UHooGameInstance::SetSelectedServerSlotIndex(int32 index)
{
	SelectedServerSlotIndex = index;
	UE_LOG(LogTemp, Warning, TEXT("Set : SelectedServerSlotIndex : %d"), SelectedServerSlotIndex);
	
}

int32 UHooGameInstance::GetSelectedServerSlotIndex()
{
	UE_LOG(LogTemp, Warning, TEXT("return : SelectedServerSlotIndex : %d"), SelectedServerSlotIndex);

	return SelectedServerSlotIndex;
}

void UHooGameInstance::GameStart()
{
	// 선택한 맵으로 이동
	UE_LOG(LogTemp, Warning, TEXT("ServerTravel : %s"), *SelectedMapURL);
	GetWorld()->ServerTravel(SelectedMapURL + "?Listen");
}

void UHooGameInstance::InitializeMaps()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Map1Image(
		TEXT("/Game/UI/MainMenu/MapImages/SnowCastleMapImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Map1OverviewImage(
		TEXT("/Game/UI/MainMenu/MapImages/SnowCastleMapOverviewImage"));
	if (Map1Image.Object)
	{
		FMapInfo Map;
		Map.MapName = "SnowCastle";
		Map.MapURL = "/Game/Levels/L_SnowCastle";
		Map.MapImage = Map1Image.Object;
		Map.MapOverviewImage = Map1OverviewImage.Object;
		MapList.Add(Map);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> Map2Image(TEXT("/Game/UI/MainMenu/MapImages/AnimMapImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Map2OverviewImage(
		TEXT("/Game/UI/MainMenu/MapImages/AnimMapOverviewImage"));
	if (Map2Image.Object)
	{
		FMapInfo Map;
		Map.MapName = "Anim";
		Map.MapURL = "/Game/Levels/L_Anim";
		Map.MapImage = Map2Image.Object;
		Map.MapOverviewImage = Map2OverviewImage.Object;
		MapList.Add(Map);
	}
}

void UHooGameInstance::CCC()
{
	FString DisplayName;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
			if (UserId.IsValid())
			{
				DisplayName = Identity->GetPlayerNickname(*UserId);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Player Display Name: %s"), *DisplayName);
}
