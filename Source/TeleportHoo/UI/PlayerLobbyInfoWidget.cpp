#include "PlayerLobbyInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Serialization/ObjectReader.h"

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

	// 플레이어 이름 업데이트
	if (PlayerName)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerName->SetText"));
		PlayerName->SetText(FText::FromString(PlayerInfo.PlayerName));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerName<-쓰레기"));
	}

	// AvatarImageData를 UTexture2D로 변환
	UTexture2D* NewAvatarImage = nullptr;
	if(PlayerInfo.AvatarImageData.Num() > 0)
	{
		// 바이트 배열을 UTextur2D로 변환
		UTexture2D* AvatarTexture = NewObject<UTexture2D>(UTexture2D::StaticClass());
		FObjectReader ObjectReader(AvatarTexture, PlayerInfo.AvatarImageData);

		// FMemoryReader FromBinary(PlayerInfo.AvatarImageData, true);
		// FromBinary.Seek(0);
		// NewAvatarImage = NewObject<UTexture2D>();
		// NewAvatarImage->Serialize(FromBinary);
		
		// FMemoryReader MemoryReader(PlayerInfo.AvatarImageData, true); // FMemoryReader 사용
		// NewAvatarImage = NewObject<UTexture2D>();
		// NewAvatarImage->Serialize(MemoryReader);
		
		if (NewAvatarImage && NewAvatarImage->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("아바타 이미지를 성공적으로 변환했습니다."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("아바타 이미지 변환에 실패했습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AvatarImageData가 비어 있습니다."));
	}

	// 아바타 이미지 업데이트
	if (AvatarImage && AvatarImage->IsValidLowLevel())
	{
		if (NewAvatarImage)
		{
			AvatarImage->SetBrushFromTexture(NewAvatarImage);
			UE_LOG(LogTemp, Warning, TEXT("AvatarImage가 성공적으로 업데이트되었습니다."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NewAvatarImage가 유효하지 않아 업데이트하지 않았습니다."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AvatarImage가 유효하지 않습니다."));
		// 기본 아바타 설정 로직 추가 (주석 처리)
		// static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultAvatar(TEXT("Path/To/DefaultAvatar"));
		// if (DefaultAvatar.Succeeded())
		// {
		// 	AvatarImage->SetBrushFromTexture(DefaultAvatar.Object);
		// }
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
