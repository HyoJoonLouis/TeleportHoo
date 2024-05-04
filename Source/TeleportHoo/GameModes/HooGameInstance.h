#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "HooGameInstance.generated.h"

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
	void CreateServer();
	UFUNCTION(BlueprintCallable)
	void JoinServer();
	
protected:
	// VARIABLES
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};