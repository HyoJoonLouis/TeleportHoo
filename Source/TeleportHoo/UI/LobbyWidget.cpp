#include "LobbyWidget.h"
#include  "Components/TextBlock.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
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

	if(TargetWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetWidget->UpdatePlayerInfo"));
		TargetWidget->UpdatePlayerInfo(PlayerInfo);
	}
}
