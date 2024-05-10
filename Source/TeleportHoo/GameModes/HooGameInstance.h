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
	int32 MaxPlayers;
	UPROPERTY(BlueprintReadWrite)
	bool IsLan;
};

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerCountsString;
	UPROPERTY(BlueprintReadOnly)
	bool IsLan;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingDel, bool, SearchingForServerDel);

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
	
protected:
	// VARIABLES
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName MySessionName;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;
	UPROPERTY(BlueprintAssignable)
	FServerSearchingDel SearchingForServerDel;
};