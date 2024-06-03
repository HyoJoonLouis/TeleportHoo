#include "PlayerLobbyInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UPlayerLobbyInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UPlayerLobbyInfoWidget::NativeConstruct"));
}

void UPlayerLobbyInfoWidget::UpdatePlayerInfo(const FPlayerInfo& PlayerInfo)
{
	UE_LOG(LogTemp, Error, TEXT("UPlayerLobbyInfoWidget::UpdatePlayerInfo 진입"));
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerInfo: PlayerName: %s, bIsReady: %s"),
		*PlayerInfo.PlayerName, PlayerInfo.bIsReady ? TEXT("true") : TEXT("false"));

	if(PlayerName)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerName->SetText"));
		PlayerName->SetText(FText::FromString(PlayerInfo.PlayerName));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerName<-쓰레기"));
	}

	if(AvatarImage&&PlayerInfo.AvatarImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarImage->SetBrushFromTexture"));
		AvatarImage->SetBrushFromTexture(PlayerInfo.AvatarImage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarImage 세팅 실패 쓰레기값임"));
	}

	if(ReadyStatus)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReadyStatus->SetText"));
		UE_LOG(LogTemp, Error, TEXT("Set Ready!!! : %s"), PlayerInfo.bIsReady ? TEXT("true") : TEXT("false"));
		ReadyStatus->SetText(PlayerInfo.bIsReady ? FText::FromString("READY") : FText::FromString("NOT READY"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReadyStatus->쓰레기"));
	}
}