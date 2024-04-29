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
}

void AMainMenuPC::HostSession()
{
	UE_LOG(LogTemp, Warning, TEXT("HostSession"));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if(Sessions.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = true; // 랜 매치인지 여부
			SessionSettings.NumPublicConnections = 2; // 최대 플레이어 수
			SessionSettings.bShouldAdvertise = true; // 세션 광고 여부

			Sessions->CreateSession(0, L"JeonginSession", SessionSettings);
			UE_LOG(LogTemp, Warning, TEXT("CreateSession"));

			UGameplayStatics::OpenLevel(GetWorld(), "L_Lobby");
			UE_LOG(LogTemp, Warning, TEXT("OpenLevel : L_Lobby"));
		}
	}
}

void AMainMenuPC::JoinSession()
{
	UE_LOG(LogTemp, Warning, TEXT("JoinSession"));

	
}