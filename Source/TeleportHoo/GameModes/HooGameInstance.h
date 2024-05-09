#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "HooGameInstance.generated.h"

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
	int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;

	void SetPlayerCount()
	{
		PlayerCountsString = (FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);

UCLASS()
class TELEPORTHOO_API UHooGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UHooGameInstance();  
	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName ServerName, bool bSucceeded);
	virtual void OnFindSessionsComplete(bool bSucceeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// FUNCTION
	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName, FString HostName);
	UFUNCTION(BlueprintCallable)
	void JoinServer();
	
protected:
	// VARIABLES
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;
};