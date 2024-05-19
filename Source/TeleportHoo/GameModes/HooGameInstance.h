#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "HooGameInstance.generated.h"

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
		PlayerCountsString = (FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}
};

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingDel, bool, SearchingForServerDel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMapInfoDel, FString, FMapNameDel);

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

	// FUNCTION
	UFUNCTION(BlueprintCallable)
	void CreateServer(FCreateServerInfo ServerInfo);
	UFUNCTION(BlueprintCallable)
	void FindServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 ArrayIndex);
	UFUNCTION(BlueprintCallable)
	void GameStart();

	UFUNCTION(BlueprintCallable)
	void FillMapList();
	UFUNCTION(BlueprintCallable)
	class UTexture2D* GetMapImage(FString MapName);
	UFUNCTION(BlueprintCallable)
	class UTexture2D* GetMapOverviewImage(FString MapName);
	UFUNCTION(BlueprintCallable)
	void SetSelectedMap(FString MapName);
	
	UFUNCTION(BlueprintCallable)
	void SetSelectedServerSlotIndex(int32 index);
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedServerSlotIndex();

protected:
	// FUNCTION
	void InitializeMaps();

protected:
	// VARIABLES
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName MySessionName;
	TArray<FMapInfo> MapList;
	FString SelectedMapName;
	FString SelectedMapURL;
	int32 SelectedServerSlotIndex;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;
	UPROPERTY(BlueprintAssignable)
	FServerSearchingDel SearchingForServerDel;
	UPROPERTY(BlueprintAssignable)
	FMapInfoDel FMapNameDel;
};
