#include "LobbyWidget.h"

#include "Components/Button.h"
#include  "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "TeleportHoo/Controllers/LobbyPlayerController.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::NativeConstruct"));

	if(B_ReadyButton)
	{
		B_ReadyButton->OnClicked.AddDynamic(this, &ULobbyWidget::OnReadyButtonClicked);
	}
	
	// 수동으로 위젯을 초기화
	WBP_PlayerLobbyInfoWidget_1 = Cast<UPlayerLobbyInfoWidget>(GetWidgetFromName(TEXT("WBP_PlayerLobbyInfoWidget_1")));
	WBP_PlayerLobbyInfoWidget_2 = Cast<UPlayerLobbyInfoWidget>(GetWidgetFromName(TEXT("WBP_PlayerLobbyInfoWidget_2")));
	
	if (WS_WidgetSwitcher)
	{
		// 기본으로 첫 번째 위젯 활성화
		WS_WidgetSwitcher->SetActiveWidget(this);
		WS_WidgetSwitcher->SetActiveWidgetIndex(0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WS_WidgetSwitcher is null"));
	}
	
	if (WBP_PlayerLobbyInfoWidget_1)
	{
		UE_LOG(LogTemp, Warning, TEXT("WBP_PlayerLobbyInfoWidget_1 initialized"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WBP_PlayerLobbyInfoWidget_1 is null"));
	}

	if (WBP_PlayerLobbyInfoWidget_2)
	{
		UE_LOG(LogTemp, Warning, TEXT("WBP_PlayerLobbyInfoWidget_2 initialized"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WBP_PlayerLobbyInfoWidget_2 is null"));
	}
}

void ULobbyWidget::UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::UpdatePlayerInfo 진입"));

	 UPlayerLobbyInfoWidget* TargetWidget = nullptr;

	// PlayerIndex 따라서 업데이트할 위젯 선택
	if(PlayerIndex == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget = WBP_PlayerLobbyInfoWidget_1"));
		TargetWidget = WBP_PlayerLobbyInfoWidget_1;
	}
	else if(PlayerIndex == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget = WBP_PlayerLobbyInfoWidget_2"));
		TargetWidget = WBP_PlayerLobbyInfoWidget_2;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerIndex 이상함"));
	}

	if(TargetWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget->UpdatePlayerInfo"));
		TargetWidget->UpdatePlayerInfo(PlayerInfo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget is nullptr"));
	}
}

void ULobbyWidget::OnReadyButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("레디 버튼 클릭됨"));

	if(APlayerController* PC = GetOwningPlayer())
	{
		if(ALobbyPlayerController* LPC = Cast<ALobbyPlayerController>(PC))
		{
			LPC->ToggleReady();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Owningplayer가 유효하지 않음"));
	}
}