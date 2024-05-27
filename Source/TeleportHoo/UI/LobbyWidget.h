#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerLobbyInfoWidget.h"
#include "LobbyWidget.generated.h"

UCLASS()
class TELEPORTHOO_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	// 플레이어 정보 갱신 함수
	void UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UPlayerLobbyInfoWidget* WBP_PlayerLobbyInfoWidget_1;

	UPROPERTY(meta = (BindWidget))
	class UPlayerLobbyInfoWidget* WBP_PlayerLobbyInfoWidget_2;
};