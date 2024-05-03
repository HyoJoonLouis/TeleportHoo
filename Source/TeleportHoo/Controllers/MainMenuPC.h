#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuPC.generated.h"

UCLASS()
class TELEPORTHOO_API AMainMenuPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuPC(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void StartOnlineGmae();
	UFUNCTION(BlueprintCallable, Category = "Network|Test")
	void FindOnlineGames();

protected:	
	// Functions
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);
	void OnFindSessionsComplete(bool bWasSuccessful);

	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// VARIABLES
	UPROPERTY()
	class UUserWidget* MainMenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")

	class TSubclassOf<UUserWidget> MainMenuWidgetClass;
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	// Delegates
		// CreateSession
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
		// StartSession
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
		// FindSession
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
		// JoinSession
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
		// DestroySession
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
};