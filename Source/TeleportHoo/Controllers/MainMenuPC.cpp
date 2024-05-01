#include "MainMenuPC.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

AMainMenuPC::AMainMenuPC()
{
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuPC Constructor"));
}

void AMainMenuPC::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AMainMenuPC BeginPlay"));

	this->bShowMouseCursor = true;

	MainMenuWidget = CreateWidget(this, MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	UE_LOG(LogTemp, Warning, TEXT("CreateWidget : MainMenuWidget"));
}

void AMainMenuPC::HostSession()
{
	UE_LOG(LogTemp, Warning, TEXT("HostSession"));

	//bool OnSuccess;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
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
			// OnSuccess = Sessions->CreateSession(0, L"JeonginSession", SessionSettings);
			UE_LOG(LogTemp, Warning, TEXT("CreateSession"));
	
			 UGameplayStatics::OpenLevel(GetWorld(), "L_Lobby");
			 UE_LOG(LogTemp, Warning, TEXT("OpenLevel : L_Lobby"));

			// 세션 생성 성공 여부에 따라 다른 로그를 출력합니다.
			//if (OnSuccess)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("CreateSession succeeded."));
			//}
			//else
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("CreateSession failed."));
			//}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : Sessions.IsValid"));
		}
	}

	// if (OnSuccess)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("CreateSession : Success"));
	// 	UGameplayStatics::OpenLevel(GetWorld(), "L_Lobby");
	// 	UE_LOG(LogTemp, Warning, TEXT("OpenLevel : L_Lobby"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("CreateSession : Failed"));
	// }
}

void AMainMenuPC::JoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinSession"));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			//세션 검색 객체 생성
			TSharedPtr<FOnlineSessionSearch> SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true; // LAN 검색 여부
			SessionSearch->MaxSearchResults = 1; // 최대 검색 결과 수

			// 세션 검색 시작
			Sessions->FindSessions(0, SessionSearch.ToSharedRef());

			// UGameplayStatics::OpenLevel(GetWorld(), "L_Lobby");
			// UE_LOG(LogTemp, Warning, TEXT("OpenLevel : L_Lobby"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed : Sessions.IsValid"));
		}
	}
}
