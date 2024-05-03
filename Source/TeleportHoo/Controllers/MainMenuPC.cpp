#include "MainMenuPC.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

// 세션 생성을 위한 성공 및 실패 콜백
FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
FDelegateHandle OnCreateSessionCompleteDelegateHandle;

FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
FDelegateHandle OnFindSessionsCompleteDelegateHandle;

FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
FDelegateHandle OnJoinSessionCompleteDelegateHandle;

AMainMenuPC::AMainMenuPC()
{
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuPC Constructor"));
	
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub != nullptr)
	{
		Sessions = OnlineSub->GetSessionInterface();
	}
}

void AMainMenuPC::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuPC BeginPlay"));

	this->bShowMouseCursor = true;

	MainMenuWidget = CreateWidget(this, MainMenuWidgetClass);
	UE_LOG(LogTemp, Warning, TEXT("CreateWidget : MainMenuWidget"));
	if (IsValid(MainMenuWidget))
	{
		MainMenuWidget->AddToViewport();
	}
}

void AMainMenuPC::HostSession()
{
	UE_LOG(LogTemp, Warning, TEXT("HostSession"));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			OnCreateSessionCompleteDelegate
				= FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnCreateSessionComplete);
			OnCreateSessionCompleteDelegateHandle 
				= Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			FOnlineSessionSettings SessionSettings;
			SessionSettings.NumPublicConnections = 2;					// 공개 세션에서 허용되는 플레이어 최대 수
			SessionSettings.NumPrivateConnections = 2;					// 비공개 세션에서 허용되는 플레이어 최대 수
			SessionSettings.bIsLANMatch = true;							// 세션이 LAN 매치인지 여부를 나타냄. true면 LAN 매치.
			SessionSettings.bAllowInvites = true; 						// 친구 초대가 허용되는지 여부
			SessionSettings.bIsDedicated = false; 						// 이 세션에 전용 서버를 사용하는지 여부
			SessionSettings.bUsesPresence = true; 						// 플레이어의 온라인 상태(접속 중, 게임 중 등)가 세션에 영향을 미치는지 여부
			SessionSettings.bUseLobbiesIfAvailable = true;				// 가능한 경우 로비를 사용할지 여부
			SessionSettings.bAllowJoinViaPresence = true;				// 플레이어의 온라인 상태를 통한 세션 참여가 가능한지 여부
			SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;	// 친구의 온라인 상태를 통해서만 세션 참여가 가능한지 여부
			SessionSettings.bAntiCheatProtected = false; 				// 안티 치트 보호가 활성화되어 있는지 여부
			SessionSettings.bUsesStats = false; 						// 통계 정보 사용 여부. 플레이어의 게임 내 통계를 추적할 필요가 있으면 true로 설정.
			SessionSettings.bShouldAdvertise = true; 					// 세션이 다른 플레이어에게 광고(노출)되는지 여부
			SessionSettings.bUseLobbiesVoiceChatIfAvailable = false;	// 가능한 경우 로비에서 음성 채팅을 사용할지 여부

			Sessions->CreateSession(0, L"JeonginSession", SessionSettings);
			UE_LOG(LogTemp, Warning, TEXT("CreateSession"));
		}
	}
}

void AMainMenuPC::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "L_Lobby", true, "listen");
		UE_LOG(LogTemp, Warning, TEXT("OpenLevel : L_Lobby"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSession Failed"));
	}
}

void AMainMenuPC::FindSession()
{
	UE_LOG(LogTemp, Warning, TEXT("FindSession"));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true; // LAN에서만 검색
			SessionSearch->MaxSearchResults = 20; // 최대 20개의 결과
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); // 온라인 상태에 따른 검색

			OnFindSessionsCompleteDelegate
				= FOnFindSessionsCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnFindSessionsComplete);
			OnFindSessionsCompleteDelegateHandle
				= Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(0, SessionSearch.ToSharedRef());
			UE_LOG(LogTemp, Warning, TEXT("Searching for sessions..."));
		}
	}
}

void AMainMenuPC::JoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinSession"));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			OnJoinSessionCompleteDelegate 
				= FOnJoinSessionCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnJoinSessionComplete);
			OnJoinSessionCompleteDelegateHandle 
				= Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// 세션 검색 및 참여 코드
			
			////세션 검색 객체 생성
			//TSharedPtr<FOnlineSessionSearch> SessionSearch = MakeShareable(new FOnlineSessionSearch());
			//SessionSearch->bIsLanQuery = true; // LAN 검색 여부
			//SessionSearch->MaxSearchResults = 1; // 최대 검색 결과 수
		}
	}
}

void AMainMenuPC::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete: %s"), bWasSuccessful ? TEXT("TRUE") : TEXT("FALSE"));
   
    if (bWasSuccessful && SessionSearch.IsValid() && SessionSearch->SearchResults.Num() > 0)
    {
    	// 가장 첫 번째 검색 결과에 참여 시도
    	Sessions->JoinSession(0, L"JeonginSession", SessionSearch->SearchResults[0]);
    }
    else
    {
    	UE_LOG(LogTemp, Warning, TEXT("No sessions found or search unsuccessful."));
    }
}

void AMainMenuPC::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Succeed : OnJoinSessionComplete"));

		APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		FString JoinAddress;

		if (PlayerController && Sessions->GetResolvedConnectString(SessionName, JoinAddress))
		{
			PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed : OnJoinSessionComplete"));
	}
}