#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "interfaces/IHttpRequest.h"
#include "interfaces/IHttpResponse.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "HooGameInstance.generated.h"

// Structs
USTRUCT(BlueprintType)
struct FCreateServerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString ServerName;

	UPROPERTY(BlueprintReadWrite)
	FString ServerMapName;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayers;
};

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly)
	FString ServerMapName;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerCountsString;

	UPROPERTY(BlueprintReadOnly)
	FString SelectedMapName;

	UPROPERTY(BlueprintReadOnly)
	int32 Ping;

	UPROPERTY(BlueprintReadOnly)
	int32 ServerArrayIndex;

	int32 CurrentPlayers;
	int32 MaxPlayers;

	void SetPlayerCount()
	{
		PlayerCountsString = FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers);
	}
};

// Map Information
USTRUCT()
struct FMapInfo
{
	GENERATED_BODY()

public:
	FString MapName;
	FString MapURL;
	class UTexture2D* MapImage;
	class UTexture2D* MapOverviewImage;
};

// PlayerSlot Information
USTRUCT(BlueprintType)
struct FMyStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FString PlayerName;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTexture2D* AvatarImage;
};


// CreateLobby_DB
USTRUCT(BlueprintType)
struct FLobbyCreationJSON
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString UserId;

	UPROPERTY(BlueprintReadWrite)
	FString Ip;
};

// JoinLobby_DB
USTRUCT(BlueprintType)
struct FLobbyJoinJSON
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString UserId;

	UPROPERTY(BlueprintReadWrite)
	int32 Idx;
};

// ScoreBoard_DB
USTRUCT(BlueprintType)
struct FMatchResultJSON
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Idx;

	UPROPERTY(BlueprintReadWrite)
	FString Win;					// 승자 UserId

	UPROPERTY(BlueprintReadWrite)
	FString Lose;					// 패자 UserId
};

// LobbyInfo_DB
USTRUCT(Blueprintable)
struct FLobbyInfoJSON
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 Idx;

	UPROPERTY(BlueprintReadOnly)
	FString HostName;

	UPROPERTY(BlueprintReadOnly)
	FString HostIP;

	UPROPERTY(BlueprintReadOnly)
	FString ClientName;

	UPROPERTY(BlueprintReadOnly)
	FString CreatedTime;
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingDel, bool, SearchingForServerDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapInfoDel, FString, FMapNameDel);

// Game Instance Class
UCLASS()
class TELEPORTHOO_API UHooGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHooGameInstance();

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName SessionName, bool bSucceeded);
	virtual void OnFindSessionsComplete(bool bSucceeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Server Functions
	UFUNCTION(BlueprintCallable)
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void FindServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 ArrayIndex);
	UFUNCTION(BlueprintCallable)
	void GameStart();
	
	// CreateServerInfo Functions
	UFUNCTION(BlueprintCallable)
	void SetCreateServerInfo(FString ServerName, FString ServerMapName, int32 MaxPlayer);
	UFUNCTION(BlueprintCallable)
	FString GetCreateServerName() const;
	
	// Map Functions
	UFUNCTION(BlueprintCallable)
	void FillMapList();
	UFUNCTION(BlueprintCallable)
	void SetSelectedMap(FString MapName);
	UFUNCTION(BlueprintCallable)
	class UTexture2D* GetMapImage(FString MapName);
	UFUNCTION(BlueprintCallable)
	class UTexture2D* GetMapOverviewImage(FString MapName);
	UFUNCTION(BlueprintCallable)
	FString GetSelectedMapName();

	// ServerSlot Functions
	UFUNCTION(BlueprintCallable)
	void SetSelectedServerSlotIndex(int32 index);
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedServerSlotIndex();

	// Matchmaking Functions
	UFUNCTION(BlueprintCallable)
	void CreateLobby_DB(const FLobbyCreationJSON& LobbyCreationData);	// lobby, (post)
	UFUNCTION(BlueprintCallable)
	void JoinLobby_DB(const FLobbyJoinJSON& LobbyJoinData);	// joinlobby, (post)
	UFUNCTION(BlueprintCallable)
	void LeaveLobby_DB(const FLobbyJoinJSON& LobbyJoinData);	// leavelobby, (post)
	UFUNCTION(BlueprintCallable)
	void MatchEnd_DB(const FMatchResultJSON& MatchResultJSON);	// matchend, (post)
	UFUNCTION(BlueprintCallable)
	void GetLobbyList_DB();	// match, (get)
	UFUNCTION(BlueprintCallable)
	void GetPlayerScore_DB(FString UserId);	// score, (get)

	// Get Functions
	UFUNCTION(BlueprintCallable)
	FString GetSteamID();
	UFUNCTION(BlueprintCallable)
	FString GetLocalIP();
	
protected:
	// Map Initialization
	void InitializeMaps();

	// Matchmaking Response
	void OnCreateLobbyResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful);
	void OnJoinLobbyResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful);
	void OnLeaveLobbyResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful);
	void OnMatchEndResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful);
	void OnGetLobbyListResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful);
	void OnGetPlayerScoreResponse(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bWasSuccessful);
	
	// Variables	/////////////////////////////////////////////
protected:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName MySessionName;

	// Map Data
	TArray<FMapInfo> MapList;
	FString SelectedMapName;
	FString SelectedMapURL;
	int32 SelectedServerSlotIndex;

	// Server Creation Data
	FCreateServerInfo CreateServerInfo;

	// Matchmaking
	TArray<FLobbyInfoJSON> ExistingLobbies;
	bool bIsCreatingLobby ;		// 상태 플래그 변수

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;
	UPROPERTY(BlueprintAssignable)
	FServerSearchingDel SearchingForServerDel;
	UPROPERTY(BlueprintAssignable)
	FMapInfoDel FMapNameDel;
};