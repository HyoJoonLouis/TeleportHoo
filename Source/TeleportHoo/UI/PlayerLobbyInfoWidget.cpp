#include "PlayerLobbyInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPlayerLobbyInfoWidget::UpdatePlayerInfo(const FPlayerInfo& PlayerInfo)
{
	if(PlayerName)
	{
		PlayerName->SetText(FText::FromString(PlayerInfo.PlayerName));
	}

	if(AvatarImage&&PlayerInfo.AvatarImage)
	{
		AvatarImage->SetBrushFromTexture(PlayerInfo.AvatarImage);
	}

	if(ReadyStatus)
	{
		ReadyStatus->SetText(PlayerInfo.bIsReady ? FText::FromString("READY") : FText::FromString("NOT READY"));
	}
}