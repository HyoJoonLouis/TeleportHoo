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
	void UpdatePlayerInfo(int32 PlayerIndex, const FPlayerInfo& PlayerInfo);

	UFUNCTION()
	void OnReadyButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	class UPlayerLobbyInfoWidget* WBP_PlayerLobbyInfoWidget_1;
	UPROPERTY(meta = (BindWidget))
	class UPlayerLobbyInfoWidget* WBP_PlayerLobbyInfoWidget_2;

	UPROPERTY(meta = (BindWidget))
	class UButton* B_ReadyButton;
};