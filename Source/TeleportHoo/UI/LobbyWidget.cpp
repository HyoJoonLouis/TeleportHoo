#include "LobbyWidget.h"
#include  "Components/TextBlock.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("ULobbyWidget::NativeConstruct"));

	
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
