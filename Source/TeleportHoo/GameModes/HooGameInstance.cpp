// Fill out your copyright notice in the Description page of Project Settings.

#include "HooGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>

UHooGameInstance::UHooGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("UHooGameInstance Constructor"));
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

void UHooGameInstance::OnCreateSessionComplete(FName ServerName, bool bSucceeded)
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
	if (bSucceeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		for(FOnlineSessionSearchResult Result : SearchResults)
		{
			if(!Result.IsValid())
				continue;

			FServerInfo Info;
			Info.ServerName = "Test Server Name";
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers -  Result.Session.NumOpenPublicConnections;
			ServerListDel.Broadcast(Info);
		}
		
		
		if (SearchResults.Num())
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

void UHooGameInstance::CreateServer()
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

	SessionInterface->CreateSession(0, FName("Session"), SessionSettings);
}

void UHooGameInstance::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinServer"));

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
