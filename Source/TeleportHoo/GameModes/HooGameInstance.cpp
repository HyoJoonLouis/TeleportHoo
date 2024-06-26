#include "HooGameInstance.h"

#include "Online.h"
#include "Engine/World.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "JsonUtils/JsonPointer.h"
#include "UObject/ConstructorHelpers.h"

// Constructor
UHooGameInstance::UHooGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("UHooGameInstance Constructor"));

	MySessionName = FName("My Session");

	InitializeMaps();

	SelectedMapName = "SnowCastle";
	SelectedMapURL = "/Game/Levels/L_SnowCastle3";

	CreateServerInfo.ServerName = "DefaultServer";
	CreateServerInfo.ServerMapName = "DefaultMap";
	CreateServerInfo.MaxPlayers = 2;
}

// Initialize
void UHooGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind Delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UHooGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UHooGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UHooGameInstance::OnJoinSessionComplete);
		}
	}
}

// Session Creation Complete Callback
void UHooGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	if (bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/Levels/L_Lobby?listen");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed: OnCreateSessionComplete"));
	}
}

// Session Find Complete Callback
void UHooGameInstance::OnFindSessionsComplete(bool bSucceeded)
{
	SearchingForServerDel.Broadcast(false);
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), bSucceeded);

	if (bSucceeded && SessionSearch->SearchResults.Num() > 0)
	{
		int32 ArrayIndex = -1;

		for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
		{
			++ArrayIndex;
			if (!Result.IsValid()) continue;

			FServerInfo Info;
			FString ServerName = "Empty Server Name";
			FString ServerMapName = "Empty Server MapName";

			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
			Result.Session.SessionSettings.Get(FName("SERVER_MAPNAME_KEY"), ServerMapName);

			Info.ServerName = ServerName;
			Info.ServerMapName = ServerMapName;
			UE_LOG(LogTemp, Warning, TEXT("ServerMapName: %s"), *Info.ServerMapName);

			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			Info.SetPlayerCount();
			Info.SelectedMapName = SelectedMapName;
			Info.Ping = Result.PingInMs;
			Info.ServerArrayIndex = ArrayIndex;

			ServerListDel.Broadcast(Info);
			UE_LOG(LogTemp, Warning, TEXT("Ping: %d"), Info.Ping);
		}

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SessionSearch->SearchResults.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed: OnFindSessionsComplete"));
	}
}

// Session Join Complete Callback
void UHooGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (!JoinAddress.IsEmpty())
		{
			PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed: JoinAddress is empty"));
		}
	}
}

// Create Server
void UHooGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("CreateServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;

	// Steam이 가능하다면 Steam으로 불가능하다면 Lan으로
	if (IOnlineSubsystem::Get()->GetSubsystemName() != "NULL")
	{
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bUseLobbiesIfAvailable = true;
		UE_LOG(LogTemp, Warning, TEXT("CreateServer -> IsSteam"));
	}
	else
	{
		SessionSettings.bIsLANMatch = true;
		SessionSettings.bUseLobbiesIfAvailable = false;
		UE_LOG(LogTemp, Warning, TEXT("CreateServer -> IsLan"));
	}

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = CreateServerInfo.MaxPlayers;
	SessionSettings.Set(L"SERVER_NAME_KEY", CreateServerInfo.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(L"SERVER_MAPNAME_KEY", CreateServerInfo.ServerMapName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

// Find Server
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
		SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("FindServer -> IsLan"));
	}
	SessionSearch->MaxSearchResults = 1000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	UE_LOG(LogTemp, Warning, TEXT("Initiating FindSessions call"));
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

// Join Server
void UHooGameInstance::JoinServer(int32 ArrayIndex)
{
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
	if (Result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Joining server at index: %d"), ArrayIndex);
		SessionInterface->JoinSession(0, MySessionName, Result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join server at index: %d"), ArrayIndex);
	}
}

// Fill Map List
void UHooGameInstance::FillMapList()
{
	MapList.Empty();
	InitializeMaps();

	for (FMapInfo Map : MapList)
	{
		FMapNameDel.Broadcast(Map.MapName);
	}
}

// Get Map Image
UTexture2D* UHooGameInstance::GetMapImage(FString MapName)
{
	for (FMapInfo Map : MapList)
	{
		if (Map.MapName.Equals(MapName))
		{
			return Map.MapImage;
		}
	}
	return nullptr;
}

// Get Map Overview Image
UTexture2D* UHooGameInstance::GetMapOverviewImage(FString MapName)
{
	for (FMapInfo Map : MapList)
	{
		if (Map.MapName.Equals(MapName))
		{
			return Map.MapOverviewImage;
		}
	}
	return nullptr;
}

// Set Selected Map
void UHooGameInstance::SetSelectedMap(FString MapName)
{
	for (FMapInfo Map : MapList)
	{
		if (Map.MapName.Equals(MapName))
		{
			SelectedMapName = Map.MapName;
			SelectedMapURL = Map.MapURL;
			UE_LOG(LogTemp, Warning, TEXT("SelectedMapName: %s"), *SelectedMapName);
			UE_LOG(LogTemp, Warning, TEXT("SelectedMapURL: %s"), *SelectedMapURL);
		}
	}
}

// Get Selected Map Name
FString UHooGameInstance::GetSelectedMapName()
{
	return SelectedMapName;
}

// Set Selected Server Slot Index
void UHooGameInstance::SetSelectedServerSlotIndex(int32 index)
{
	SelectedServerSlotIndex = index;
	UE_LOG(LogTemp, Warning, TEXT("Set: SelectedServerSlotIndex: %d"), SelectedServerSlotIndex);
}

// Get Selected Server Slot Index
int32 UHooGameInstance::GetSelectedServerSlotIndex()
{
	UE_LOG(LogTemp, Warning, TEXT("Return: SelectedServerSlotIndex: %d"), SelectedServerSlotIndex);
	return SelectedServerSlotIndex;
}

void UHooGameInstance::CreateLobby_DB(const FMatchMakingJSON& MatchMakingData)
{
	// HTTP 요청을 생성하고, 콜백함수 바인딩, URL과 요청 메서드를 설정
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHooGameInstance::OnCreateLobbyResponse);
	Request->SetURL("https://13.125.214.134/lobby");		//	13.125.214.134
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	// JSON 객체를 생성하고, 생성한 JSON에 값 설정
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("userId", MatchMakingData.UserId);
	JsonObject->SetStringField("ip", MatchMakingData.Ip);

	// JSON 객체를 문자열로 변환해 서버로 전송
	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

void UHooGameInstance::JoinLobby_DB(const FMatchJoinJSON& MatchJoinData)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHooGameInstance::OnJoinLobbyResponse);
	Request->SetURL("https://13.125.214.134/matchjoin");		//	13.125.214.134
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("userId", MatchJoinData.UserId);
	JsonObject->SetNumberField("idx", MatchJoinData.Idx);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

void UHooGameInstance::MatchEnd_DB(const FFinishMatchJSON& FinishMatchData)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UHooGameInstance::OnMatchEndResponse);
	Request->SetURL("https://13.125.214.134/matchend");		//	13.125.214.134
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetNumberField("idx", FinishMatchData.Idx);
	JsonObject->SetStringField("win", FinishMatchData.Win);
	JsonObject->SetStringField("lose", FinishMatchData.Lose);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

void UHooGameInstance::GetLobbyList_DB()
{
	
}

void UHooGameInstance::GetPlayerScore_DB(FString UserId)
{
	
}

FString UHooGameInstance::GetSteamID()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
		if(Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(0);
			if(UserId.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("UserId: %s"), *UserId->ToString());
				return UserId->ToString();
			}
		}
	}

	return FString("InvalidSteamID");
}

FString UHooGameInstance::GetLocalIP()
{
    bool bCanBindAll;
    TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);

    if (Addr->IsValid())
    {
		UE_LOG(LogTemp, Warning, TEXT("LocalIP: %s"), *Addr->ToString(false));
        return Addr->ToString(false);
    }
	
    return FString("InvalidIP");
}

// Game Start
void UHooGameInstance::GameStart()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerTravel: %s"), *SelectedMapURL);
	GetWorld()->ServerTravel(SelectedMapURL + "?Listen");
}

// Set Create Server Info
void UHooGameInstance::SetCreateServerInfo(FString ServerName, FString ServerMapName, int32 MaxPlayer)
{
	CreateServerInfo.ServerName = ServerName;
	CreateServerInfo.ServerMapName = ServerMapName;
	CreateServerInfo.MaxPlayers = MaxPlayer;
}

// Get Create Server Name
FString UHooGameInstance::GetCreateServerName() const
{
	UE_LOG(LogTemp, Warning, TEXT("UHooGameInstance::GetCreateServerName called"));
	return CreateServerInfo.ServerName;
}

// Initialize Maps
void UHooGameInstance::InitializeMaps()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> Map1Image(TEXT("/Game/UI/MainMenu/MapImages/SnowCastleMapImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Map1OverviewImage(TEXT("/Game/UI/MainMenu/MapImages/SnowCastleMapOverviewImage"));
	if (Map1Image.Object)
	{
		FMapInfo Map;
		Map.MapName = "SnowCastle";
		Map.MapURL = "/Game/Levels/L_SnowCastle3";
		Map.MapImage = Map1Image.Object;
		Map.MapOverviewImage = Map1OverviewImage.Object;
		MapList.Add(Map);
	}

	static ConstructorHelpers::FObjectFinder<UTexture2D> Map2Image(TEXT("/Game/UI/MainMenu/MapImages/AnimMapImage"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Map2OverviewImage(TEXT("/Game/UI/MainMenu/MapImages/AnimMapOverviewImage"));
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

void UHooGameInstance::OnCreateLobbyResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful)
{
	if (bWasSuccessful && HttpResponse->GetResponseCode() == 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateLobby succeeded: %s"), *HttpResponse->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CreateLobby failed"));
	}
}

void UHooGameInstance::OnJoinLobbyResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful)
{
	if (bWasSuccessful && HttpResponse->GetResponseCode() == 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinLobby succeeded: %s"), *HttpResponse->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JoinLobby failed"));
	}
}

void UHooGameInstance::OnMatchEndResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful)
{
	if (bWasSuccessful && HttpResponse->GetResponseCode() == 200)
	{
		UE_LOG(LogTemp, Warning, TEXT("MatchEnd succeeded: %s"), *HttpResponse->GetContentAsString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MatchEnd failed"));
	}
}