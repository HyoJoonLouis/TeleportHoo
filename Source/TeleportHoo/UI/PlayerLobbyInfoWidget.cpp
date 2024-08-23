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
	UE_LOG(LogTemp, Warning, TEXT("UPlayerLobbyInfoWidget::UpdatePlayerInfo 진입"));
	UE_LOG(LogTemp, Warning, TEXT("UpdatePlayerInfo: PlayerName: %s, bIsReady: %s"),
	       *PlayerInfo.PlayerName, PlayerInfo.bIsReady ? TEXT("true") : TEXT("false"));

	if (PlayerName)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerName->SetText"));
		PlayerName->SetText(FText::FromString(PlayerInfo.PlayerName));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerName<-쓰레기"));
	}
	
	if (AvatarImage)
	{
		if (PlayerInfo.AvatarImage)
		{
			UE_LOG(LogTemp, Error, TEXT("AvatarImage->SetBrushFromTexture : 아바타 이미지 설정 성공"));
			AvatarImage->SetBrushFromTexture(PlayerInfo.AvatarImage);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerInfo.AvatarImage가 유효하지 않습니다. 기본 아바타를 설정합니다."));
			// 기본 아바타 설정 로직 추가
			// static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultAvatar(TEXT("Path/To/DefaultAvatar"));
			// if (DefaultAvatar.Succeeded())
			// {
			// 	AvatarImage->SetBrushFromTexture(DefaultAvatar.Object);
			// }
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarImage가 유효하지 않습니다."));
	}

	if (ReadyStatus)
	{
		UE_LOG(LogTemp, Warning, TEXT("ReadyStatus->SetText"));
		UE_LOG(LogTemp, Error, TEXT("Set Ready!!! : %s"), PlayerInfo.bIsReady ? TEXT("true") : TEXT("false"));
		ReadyStatus->SetText(PlayerInfo.bIsReady ? FText::FromString("READY") : FText::FromString("NOT READY"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ReadyStatus->쓰레기"));
	}
}