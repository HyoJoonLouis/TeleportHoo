// Fill out your copyright notice in the Description page of Project Settings.

#include "HooGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>

#include "ISourceControlProvider.h"

UHooGameInstance::UHooGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("UHooGameInstance Constructor"));

	MySessionName = FName("My Session");
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
		int8 ArrayIndex = 0;
		
		for(FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			ArrayIndex++;

			if(!Result.IsValid())
				continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString HostName = "Empty Host Name";
			
			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);
			
			Info.ServerName = ServerName;
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers -  Result.Session.NumOpenPublicConnections;
			Info.ServerArrayIndex = ArrayIndex;

			Info.SetPlayerCount();
			
			ServerListDel.Broadcast(Info);
		}

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count : %d"), SessionSearch->SearchResults.Num());

		if (SessionSearch->SearchResults.Num())
		{
			// SessionInterface->JoinSession(0, FName("Session"), SearchResults[0]);
		}
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

void UHooGameInstance::CreateServer(FString ServerName, FString HostName)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;

	// 스팀, 엔진 테스트 전환하려면 DefaultEngine.ini에서 DefaultPlatformService 를 Steam, NULL 을 전환해주면 됨.
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	{
		SessionSettings.bIsLANMatch = false;
	}
	else
	{
		SessionSettings.bIsLANMatch = true;		// Is LAN
	}
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionSettings.Set(L"SERVER_NAME_KEY", ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(L"SERVER_HOSTNAME_KEY", HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
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
	}
	else
	{
		SessionSearch->bIsLanQuery = true;		// Is LAN
	}
	SessionSearch->MaxSearchResults = 1000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UHooGameInstance::JoinServer(int32 ArrayIndex)
{
	if (ArrayIndex < 0 || ArrayIndex >= SessionSearch->SearchResults.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid ArrayIndex."));
		return;
	}
	
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
	if(Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("JOINING SERVER AT INDEX : %d"), ArrayIndex);
		SessionInterface->JoinSession(0, MySessionName, Result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED TO JOIN SERVER AT INDEX : %d"), ArrayIndex);
	}
}