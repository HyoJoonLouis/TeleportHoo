#include "LobbyWidget.h"
#include "../GameModes/LobbyGameMode.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include  "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "TeleportHoo/Controllers/LobbyPlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::NativeConstruct"));

	if (B_StartButton)
	{
		B_StartButton->SetIsEnabled(false);
		B_StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnStartButtonClicked);
	}

	if (B_ReadyButton)
	{
		B_ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyButtonClicked);
	}
}

void ULobbyWidget::UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::UpdatePlayerInfo 진입"));
	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::UpdatePlayerInfo : PlayerName: %s, bIsReady: %s"),
	       *PlayerInfo.PlayerName, PlayerInfo.bIsReady ? TEXT("true") : TEXT("false"));

	UPlayerLobbyInfoWidget* TargetWidget = nullptr;

	// PlayerIndex 따라서 업데이트할 위젯 선택
	if (PlayerIndex == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget = WBP_PlayerLobbyInfoWidget_1"));
		TargetWidget = WBP_PlayerLobbyInfoWidget_1;
	}
	else if (PlayerIndex == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget = WBP_PlayerLobbyInfoWidget_2"));
		TargetWidget = WBP_PlayerLobbyInfoWidget_2;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerIndex 이상함"));
	}

	if (TargetWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget->UpdatePlayerInfo"));
		TargetWidget->UpdatePlayerInfo(PlayerInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget is nullptr"));
	}
}

void ULobbyWidget::SetStartButtonEnabled(bool bEnabled)
{
	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::SetStartButtonEnabled 진입"));
	if (B_StartButton)
	{
		B_StartButton->SetIsEnabled(bEnabled);
		UE_LOG(LogTemp, Warning, TEXT("SetStartButtonEnabled: %s"), bEnabled ? TEXT("Enabled") : TEXT("Disabled"));
	}
}

void ULobbyWidget::SetStartButtonVisibility(bool bIsVisible)
{
	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::SetStartButtonVisibility 진입"));
	if (B_StartButton)
	{
		B_StartButton->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("SetStartButtonVisibility: %s"), bIsVisible ? TEXT("Visible") : TEXT("Hidden"));
	}
}

void ULobbyWidget::SetLoadingScreen(ESlateVisibility NewVisibility)
{
	if(Border_LoadingScreen)
	{
		Border_LoadingScreen->SetVisibility(NewVisibility);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Border_LoadingScreen is nullptr"));
	}
}

void ULobbyWidget::OnStartButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("스타트 버튼 클릭됨"));

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController && PlayerController->HasAuthority())
	{
		ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
		if (LobbyGameMode)
		{
			LobbyGameMode->ShowLoadingScreenToAllPlayers();
		}
	}

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	//if (PlayerController && PlayerController->HasAuthority())
	//{
	//	ALobbyGameMode* LobbyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
	//	if (LobbyGameMode)
	//	{
	//		LobbyGameMode->StartCharacterSelection();
	//	}
	//}
}

void ULobbyWidget::OnReadyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("레디 버튼 클릭됨"));

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
		{
			LPC->ToggleReady();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owningplayer가 유효하지 않음"));
	}
}

void ULobbyWidget::SetServerName(const FString& ServerName)
{
	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::SetServerName 진입"));
	UE_LOG(LogTemp, Warning, TEXT("ServerName: %s"), *ServerName);

	if (T_ServerName)
	{
		UE_LOG(LogTemp, Warning, TEXT("T_ServerName->SetText 진입"));
		T_ServerName->SetText(FText::FromString(ServerName));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerNameTextBlock is nullptr"));
	}
}

void ULobbyWidget::SetWidgetSwitcherIndex(int32 Index)
{
	if (IsValid(WS_WidgetSwitcher))
	{
		WS_WidgetSwitcher->SetActiveWidgetIndex(Index);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetSwitcher is nullptr"));
	}
}

