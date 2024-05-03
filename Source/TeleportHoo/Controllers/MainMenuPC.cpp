#include "MainMenuPC.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

AMainMenuPC::AMainMenuPC(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 세션을 만들기위해 함수 바인딩
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &AMainMenuPC::OnDestroySessionComplete);
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

void AMainMenuPC::StartOnlineGmae()
{
	// 사용자 ID를 가져올 용도의 로컬 플레이어
	ULocalPlayer* const LocalPlayer = GetLocalPlayer();

	// GetPreferredUniqueNetId() 호출 결과를 적절히 변환
	FUniqueNetIdRepl UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();

	// FUniqueNetIdRepl에서 TSharedPtr<const FUniqueNetId, ESPMode::ThreadSafe>로 변환
	if(UniqueNetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId, ESPMode::ThreadSafe> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();

		// 변환된 UniqueNetId를 사용하여 HostSession을 호출
		HostSession(UniqueNetId, GameSessionName, true, true, 4);
	}
}

void AMainMenuPC::FindOnlineGames()
{
	// 로컬 플레이어를 사용하여 사용자 ID 가져오기
	ULocalPlayer* const LocalPlayer = GetLocalPlayer();

	// GetPreferredUniqueNetId() 호출 결과를 적절히 변환
	FUniqueNetIdRepl UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();

	// FUniqueNetIdRepl에서 TSharedPtr<const FUniqueNetId, ESPMode::ThreadSafe>로 변환
	if (UniqueNetIdRepl.IsValid())
	{
		TSharedPtr<const FUniqueNetId, ESPMode::ThreadSafe> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();

		// 변환된 UniqueNetId를 사용하여 FindSessions을 호출
		FindSessions(UniqueNetId, true, true);
	}
}

bool AMainMenuPC::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers)
{
	// 온라인 서브시스템 가져오기
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// "CreateSession" 함수 호출을 위해 세션 인터페이스 가져오기 
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, FString("NewMap"), EOnlineDataAdvertisementType::ViaOnlineService);

			// 세션 인터페이스의 Handle에 델리게이트 설정
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// 세션 생성이 완료되면 (성공 여부와 상관없이) 델리게이트가 호출됩니다.
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void AMainMenuPC::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// 온라인 서브시스템을 얻기 위해 OnlineSubsystem을 가져옵니다.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// 세션 인터페이스를 가져와서 StartSession 함수를 호출할 수 있도록 합니다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// 이 호출이 완료되었으므로 SessionComplete 델리게이트 핸들을 지웁니다.
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// StartSession 델리게이트 핸들을 설정합니다.
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// StartSession이 호출된 후에 StartSessionComplete 델리게이트가 호출됩니다.
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void AMainMenuPC::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// 온라인 서브시스템을 얻기 위해 OnlineSubsystem을 가져옵니다.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// 세션 인터페이스를 얻기 위해 OnlineSubsystem을 가져옵니다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// 이 호출이 완료되었으므로 델리게이트를 지웁니다.
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// 시작이 성공했다면, 우리는 원하는 경우 NewMap을 열 수 있습니다. "listen"을 매개변수로 사용하는지 확인하세요!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "L_Lobby", true, "listen");
	}
}

void AMainMenuPC::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence)
{
	// 사용할 온라인 서브시스템을 가져옵니다.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// OnlineSubsystem에서 세션 인터페이스를 가져옵니다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// LAN 게임을 검색하는지, 몇 개의 결과를 원하는지 등 모든 검색 설정을 채웁니다.
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// "bIsPresence"가 true인 경우에만 이 쿼리 설정을 설정합니다.
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Delegate를 FindSession 함수의 Delegate Handle로 설정합니다.
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// 세션 인터페이스 함수를 호출합니다. 이 작업이 완료되면 Delegate가 호출됩니다.
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// 문제가 발생한 경우에는 직접 Delegate 함수를 "false"로 호출합니다.
		OnFindSessionsComplete(false);
	}
}

void AMainMenuPC::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// 사용할 OnlineSubsystem을 가져옵니다.
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// OnlineSubsystem의 세션 인터페이스를 가져옵니다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// 이 호출이 완료되었으므로 델리게이트 핸들을 지웁니다.
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// 검색 결과의 수를 디버깅합니다. 이후 UMG 등에서 표시할 수 있습니다.
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// 최소한 1개의 세션이 발견된 경우에만 디버깅합니다. BP 버전에서와 같이 UMG 위젯 목록에 추가할 수 있습니다.
			if (SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults"는 모든 정보를 포함하는 배열입니다. 여기서 세션에 액세스하고 많은 정보를 가져올 수 있습니다.
				// 나중에 자체 클래스를 사용하여 더 많은 정보를 추가하고 표시할 수 있습니다.
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName은 현재 세션 이름입니다. 적절한 게임 서버 이름을 추가할 수 있는 자체 호스트 설정 클래스와 게임 세션 클래스를 만들어 여기에 추가할 수 있습니다.
					// 이것은 Blueprint에서 할 수 없는 것입니다!
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx+1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}
}

bool AMainMenuPC::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
	const FOnlineSessionSearchResult& SearchResult)
{
	// 성공 여부를 나타내는 bool 변수를 초기화합니다.
	bool bSuccessful = false;

	// 사용할 OnlineSubsystem을 가져옵니다.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// OnlineSubsystem에서 세션 인터페이스를 가져옵니다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// 다시 핸들을 설정합니다.
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// "JoinSession" 함수를 호출하여 전달된 "SearchResult"로 세션에 참가합니다.
			// "SessionSearch->SearchResults"를 사용하여 "FOnlineSessionSearchResult"를 얻고 전달할 수 있습니다.
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void AMainMenuPC::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));
    
    // 사용할 OnlineSubsystem을 가져옵니다.
    IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
    if (OnlineSub)
    {
        // OnlineSubsystem에서 세션 인터페이스를 가져옵니다.
        IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
    
        if (Sessions.IsValid())
        {
            // 다시 Delegate를 지웁니다.
            Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
    
            // 서버 맵으로 이동하기 위해 첫 번째 로컬 PlayerController를 가져옵니다.
            // 이것은 Blueprint 노드 "Join Session"이 자동으로 수행합니다!
            APlayerController* const PlayerController =  GetWorld()->GetFirstPlayerController();
    
            // ClientTravel을 사용하기 위해 FString이 필요합니다. 세션 인터페이스에 SessionName과 빈 문자열을 전달하여
            // 이러한 String을 만들도록 요청할 수 있습니다. 이렇게 하려는 이유는 모든 OnlineSubsystem이 다른 TravelURL을 사용하기 때문입니다.
            FString TravelURL;
    
            if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
            {
                // 마지막으로 ClientTravel을 호출합니다. 원한다면 TravelURL을 출력하여 실제로 어떻게 보이는지 볼 수 있습니다.
                PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
            }
        }
    }
}

void AMainMenuPC::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// 사용할 OnlineSubsystem을 가져옵니다.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// OnlineSubsystem에서 세션 인터페이스를 가져옵니다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Delegate를 지웁니다.
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// 성공한 경우, 다른 레벨을 로드합니다 (메인 메뉴가 될 수도 있습니다!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true);
			}
		}
	}
}